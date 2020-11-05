// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"
#include "UObject/UObjectHash.h"

void UMyObject::Serialize(FArchive& Ar)
{
    // if (Ar.IsLoading() || Ar.IsModifyingWeakAndStrongReferences())
    // {
    //     // Rehash since SuperStruct will be serialized in UStruct::Serialize
    //     UnhashObject(this);
    // }

    Super::Serialize(Ar);

    // if (Ar.IsLoading() || Ar.IsModifyingWeakAndStrongReferences())
    // {
    //     HashObject(this);
    // }

    Ar.ThisContainsCode();

    Ar << (float&)test;


    if (Ar.IsLoading())
    {
        checkf(!HasAnyClassFlags(CLASS_Native),
               TEXT("Class %s loaded with CLASS_Native....we should not be loading any native classes."),
               *GetFullName());
        checkf(!HasAnyClassFlags(CLASS_Intrinsic),
               TEXT("Class %s loaded with CLASS_Intrinsic....we should not be loading any intrinsic classes."),
               *GetFullName());
        ClassFlags &= ~(CLASS_ShouldNeverBeLoaded | CLASS_TokenStreamAssembled);
        if (!(Ar.GetPortFlags() & PPF_Duplicate))
        {
            Link(Ar, true);
        }
    }


    // Defaults.

    // mark the archive as serializing defaults
    Ar.StartSerializingDefaults();

    if (Ar.IsLoading())
    {
        check((Ar.GetPortFlags() & PPF_Duplicate) || (GetStructureSize() >= sizeof(UObject)));
        check(!GetSuperClass() || !GetSuperClass()->HasAnyFlags(RF_NeedLoad));

        // record the current CDO, as it stands, so we can compare against it 
        // after we've serialized in the new CDO (to detect if, as a side-effect
        // of the serialization, a different CDO was generated)
        UObject* const OldCDO = ClassDefaultObject;

        // serialize in the CDO, but first store it here (in a temporary var) so
        // we can check to see if it should be the authoritative CDO (a newer 
        // CDO could be generated as a side-effect of this serialization)
        //
        // @TODO: for USE_CIRCULAR_DEPENDENCY_LOAD_DEFERRING, do we need to 
        //        defer this serialization (should we just save off the tagged
        //        serialization data for later use)?
        UObject* PerspectiveNewCDO = NULL;
        Ar << PerspectiveNewCDO;

        // Blueprint class regeneration could cause the class's CDO to be set.
        // The CDO (<<) serialization call (above) probably will invoke class 
        // regeneration, and as a side-effect the CDO could already be set by 
        // the time it returns. So we only want to set the CDO here (to what was 
        // serialized in) if it hasn't already changed (else, the serialized
        // version could be stale). See: TTP #343166
        if (ClassDefaultObject == OldCDO)
        {
            ClassDefaultObject = PerspectiveNewCDO;
        }
            // if we reach this point, then the CDO was regenerated as a side-effect
            // of the serialization... let's log if the regenerated CDO (what's 
            // already been set) is not the same as what was returned from the 
            // serialization (could mean the CDO was regenerated multiple times?)
        else if (PerspectiveNewCDO != ClassDefaultObject)
        {
            UE_LOG(LogClass, Log,
                   TEXT(
                       "CDO was changed while class serialization.\n\tOld: '%s'\n\tSerialized: '%s'\n\tActual: '%s'"
                   )
                   , OldCDO ? *OldCDO->GetFullName() : TEXT("NULL")
                   , PerspectiveNewCDO ? *PerspectiveNewCDO->GetFullName() : TEXT("NULL")
                   , ClassDefaultObject ? *ClassDefaultObject->GetFullName() : TEXT("NULL"));
        }
        ClassUnique = 0;
    }
    else
    {
        check(!ClassDefaultObject || GetDefaultsCount()==GetPropertiesSize());

        // only serialize the class default object if the archive allows serialization of ObjectArchetype
        // otherwise, serialize the properties that the ClassDefaultObject references
        // The logic behind this is the assumption that the reason for not serializing the ObjectArchetype
        // is because we are performing some actions on objects of this class and we don't want to perform
        // that action on the ClassDefaultObject.  However, we do want to perform that action on objects that
        // the ClassDefaultObject is referencing, so we'll serialize it's properties instead of serializing
        // the object itself
        if (!Ar.IsIgnoringArchetypeRef())
        {
            Ar << ClassDefaultObject;
        }
        else if ((ClassDefaultObject != nullptr && !Ar.HasAnyPortFlags(PPF_DuplicateForPIE | PPF_Duplicate)) ||
            ClassDefaultObject != nullptr)
        {
            ClassDefaultObject->Serialize(Ar);
        }
    }

    if (!Ar.IsLoading() && !Ar.IsSaving())
    {
        if (GetSparseClassDataStruct() != nullptr)
        {
            SerializeSparseClassData(FStructuredArchiveFromArchive(Ar).GetSlot());
        }
    }

    // mark the archive we that we are no longer serializing defaults
    Ar.StopSerializingDefaults();

    if (Ar.IsLoading())
    {
        if (ClassDefaultObject == NULL)
        {
            check(GConfig);
            if (GEventDrivenLoaderEnabled)
            {
                ClassDefaultObject = GetDefaultObject();
                // we do this later anyway, once we find it and set it in the export table. 
                // ClassDefaultObject->SetFlags(RF_NeedLoad | RF_NeedPostLoad | RF_NeedPostLoadSubobjects);
            }
            else if (!Ar.HasAnyPortFlags(PPF_DuplicateForPIE | PPF_Duplicate))
            {
                UE_LOG(LogClass, Error, TEXT("CDO for class %s did not load!"), *GetPathName());
                ensure(ClassDefaultObject != NULL);
                ClassDefaultObject = GetDefaultObject();
                Ar.ForceBlueprintFinalization();
            }
        }
    }
}