// Fill out your copyright notice in the Description page of Project Settings.


#include "Global.h"

UGlobal* instance;

UGlobal* UGlobal::GetInstance()
{
    return instance;
}

UGlobal::UGlobal()
{
    UE_LOG(LogTemp, Log, TEXT("globalinit"));
    instance = this;
}


template<typename... TReturns, typename... TArgs>
void ImplementFunction(UClass* OuterClass, UFunction* Function, TTuple<TReturns...>& OutParams, TArgs&&... Args)
{
	void* FuncPramsStructAddr = (uint8*)FMemory_Alloca(Function->ParmsSize);
	uint8* OutPramsByte = (uint8*)&OutParams;
	TTuple<TArgs...> InParams(Forward<TArgs>(Args)...);
	uint8* InPramsByte = (uint8*)&InParams;

	if (Function->HasAnyFunctionFlags(FUNC_Native))
	{
		FFrame Frame(nullptr, Function, &FuncPramsStructAddr, nullptr, Function->Children);
		uint8* ReturnParams = nullptr;

		//执行无字节码的函数
		//注册虚拟机参数列表
		FOutParmRec** LastOut = &Frame.OutParms;

		for (TFieldIterator<UProperty> ParmIt(Function); ParmIt; ++ParmIt)
		{
			UProperty *Property = *ParmIt;
			if (Property->PropertyFlags & CPF_OutParm)
			{
				if (Property->PropertyFlags & CPF_ReturnParm)
				{
					ReturnParams = OutPramsByte;
					OutPramsByte += Property->GetSize();
				}
				else
				{
					for (;;)
					{
						if (*LastOut == nullptr)
						{
							(*LastOut) = (FOutParmRec*)FMemory_Alloca(sizeof(FOutParmRec));
							(*LastOut)->PropAddr = OutPramsByte;
							(*LastOut)->Property = Property;
							(*LastOut)->NextOutParm = nullptr;
							LastOut = &(*LastOut)->NextOutParm;

							OutPramsByte += Property->GetSize();

							break;
						}
						else
						{
							LastOut = &(*LastOut)->NextOutParm;
						}
					}
				}
			}
			else if (Property->PropertyFlags & CPF_Parm)
			{
				for (;;)
				{
					if (*LastOut == nullptr)
					{
						(*LastOut) = (FOutParmRec*)FMemory_Alloca(sizeof(FOutParmRec));
						(*LastOut)->PropAddr = (uint8 *)(Property->ContainerPtrToValuePtr<void*>(&FuncPramsStructAddr));
						if (UObjectProperty *ObjectPropert = Cast<UObjectProperty>(Property))
						{
							ObjectPropert->SetObjectPropertyValue(const_cast<uint8*>((*LastOut)->PropAddr), *(UObject**)InPramsByte);
						}
						else
						{
							FMemory::Memcpy((*LastOut)->PropAddr, InPramsByte, Property->GetSize());
						}

						(*LastOut)->Property = Property;
						(*LastOut)->NextOutParm = nullptr;
						LastOut = &(*LastOut)->NextOutParm;

						InPramsByte += Property->GetSize();

						break;
					}
					else
					{
						LastOut = &(*LastOut)->NextOutParm;
					}
				}
			}
		}
	
		Function->Invoke(OuterClass, Frame, ReturnParams);
		return;
	}

	//必须要有字节码，也就是它必须是蓝图函数，或者是BlueprintNativeEvent or BlueprintImplementableEvent 在蓝图编译的函数
	//遍历函数
	for (TFieldIterator<UProperty> i(Function); i; ++i)
	{
		UProperty* Property = *i;
		void* PropAddr = Property->ContainerPtrToValuePtr<void*>(FuncPramsStructAddr);
		if (Property->PropertyFlags & CPF_OutParm)
		{
			FMemory::Memcpy(PropAddr, OutPramsByte, Property->GetSize());
			OutPramsByte += Property->GetSize();
		}
		else if (Property->PropertyFlags & CPF_Parm)
		{
			FMemory::Memcpy(PropAddr, InPramsByte, Property->GetSize());
			InPramsByte += Property->GetSize();
		}
	}

	//必须要有字节码
	OuterClass->ProcessEvent(Function, FuncPramsStructAddr);

	OutPramsByte = (uint8*)&OutParams;
	for (TFieldIterator<UProperty> i(Function); i; ++i)
	{
		UProperty* Property = *i;
		if (Property->PropertyFlags & CPF_OutParm)
		{
			void* PropAddr = Property->ContainerPtrToValuePtr<void*>(FuncPramsStructAddr);
			FMemory::Memcpy(OutPramsByte, PropAddr, Property->GetSize());

			OutPramsByte += Property->GetSize();
		}
	}
}

//TReturns	包含引用 和返回值 一般返回值放在后面
//TArgs		传递参数 包含指针和变量
template<typename... TReturns, typename... TArgs>
void Call(FName FunctionName, TTuple<TReturns...>& OutParams, TArgs&&... Args)
{
	//可以找到带有字节码的函数
	UFunction* Function = (UFunction*)StaticFindObjectFast(UFunction::StaticClass(), nullptr, FunctionName, false, true, RF_Transient);
	if (!Function)
	{
		//寻找无字节码的函数 
		Function = FindObject<UFunction>(ANY_PACKAGE, *FunctionName.ToString());
	}

	ImplementFunction<TReturns...>(Function->GetOuterUClass(), Function, OutParams, Forward<TArgs>(Args)...);
}