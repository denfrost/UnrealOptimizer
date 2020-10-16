#include "OptimizerActorComponent.h"


#include "Runtime/Renderer/Public/PrimitiveSceneInfo.h"

// Sets default values for this component's properties
UOptimizerActorComponent::UOptimizerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0; // Set 0.2-0.25 for slow machine

	ArrowOptimizer = CreateDefaultSubobject<UArrowComponent>(TEXT("OptimizerArrow"));
    ArrowOptimizer->SetHiddenInGame(false,true);
	ArrowOptimizer->SetVisibility(true);
	ArrowOptimizer->ArrowColor = FColor(255, 0, 0);
	//ArrowOptimizer->SetRelativeScale3D(FVector::ZeroVector); //just for render activation
}


// Called when the game starts
void UOptimizerActorComponent::BeginPlay()
{
	Super::BeginPlay();
	FString sn = GetOwner()->GetFName().ToString();
	UE_LOG(LogTemp, Log, TEXT("Optimizer BeginPlay! %s"),*sn);
	// ...Get Info
	
	//UPrimitiveComponent* PC = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	//float snmd = PC->MinDrawDistance;
	//UE_LOG(LogTemp, Log, TEXT("MinDrawDistance :  %f"), snmd);
	ArrowOptimizer->SetWorldLocation(GetOwner()->GetActorLocation());
	GetOwner()->AddInstanceComponent(ArrowOptimizer);
	if (bShowActorsOptimizer)
	{
		ArrowOptimizer->SetRelativeScale3D(FVector::OneVector * 2); //just for render activation
	}
	else { ArrowOptimizer->SetVisibility(false); }
	
	if (!MonitorComponent)
	{
		MonitorComponent = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		if (!MonitorComponent) UE_LOG(LogTemp, Log, TEXT("Optimizer Casting problem! %s"), *GetNameSafe(GetOwner()));
	}
}


// Called every frame
void UOptimizerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bOnScreenNow = WasRecentlyVisible();
	if (bCachedVisibleNow != bOnScreenNow)
	{
		OnCheckVisible.Broadcast(bOnScreenNow);
		bCachedVisibleNow = bOnScreenNow;

		//LogInfo
		if (MonitorComponent) CheckRenderInfo(MonitorComponent);
	}

	//bRenderNow = WasRecentlyRendered(RenderDelay);
	/*if (bCachedRenderNow != bRenderNow)
	{
		OnCheckRender.Broadcast(bRenderNow, bOnScreenNow);
		bCachedRenderNow = bRenderNow;
	};*/

	//ArrowComponent->SetVisibility(!bRenderNow);
	// ...
}


//
bool UOptimizerActorComponent::CheckRenderInfo(UPrimitiveComponent* _PrimitiveComp)
{
	if (const UWorld* const World = GetWorld())
	{
		UE_LOG(LogTemp, Log, TEXT("UPrimitiveComponent RenderInfo:"));
		UE_LOG(LogTemp, Log, TEXT("Name Object: %s"),*GetNameSafe(_PrimitiveComp));
		if (_PrimitiveComp)
		{
			int32 d = _PrimitiveComp->VisibilityId;
			UE_LOG(LogTemp, Log, TEXT("VisibilityId = %d"), d);
		}
		return true;
	};
	return false;
}

bool UOptimizerActorComponent::WasRecentlyRendered(float &DelayRender,float Tolerance /*= 0.2*/) const
{
	if (const UWorld* const World = GetWorld())
	{
		// Adjust tolerance, so visibility is not affected by bad frame rate / hitches.
		const float RenderTimeThreshold = FMath::Max(Tolerance, World->DeltaTimeSeconds + KINDA_SMALL_NUMBER);

		// If the current cached value is less than the tolerance then we don't need to go look at the components
		DelayRender = World->TimeSince(ArrowOptimizer->GetLastRenderTime());
		return DelayRender <= RenderTimeThreshold; //World->TimeSince(GetOwner()->GetLastRenderTime()) <= RenderTimeThreshold;
	}
	return false;
}

bool UOptimizerActorComponent::WasRecentlyVisible(float Tolerance /*= 0.2*/) const
{
	if (const UWorld* const World = GetWorld())
	{
		// Adjust tolerance, so visibility is not affected by bad frame rate / hitches.
		const float RenderTimeThreshold = FMath::Max(Tolerance, World->DeltaTimeSeconds + KINDA_SMALL_NUMBER);

		// If the current cached value is less than the tolerance then we don't need to go look at the components
		return World->TimeSince(MonitorComponent->GetLastRenderTimeOnScreen()) <= RenderTimeThreshold;
	}
	return false;
}

/*
 * Returns true if this primitive component has been rendered "recently", with a tolerance in seconds to define what "recent" means.
 * e.g.: If a tolerance of 0.1 is used, this function will return true only if the primitive component was rendered in the last 0.1 seconds of game time.
 *
 * @param Tolerance  How many seconds ago the actor last render time can be and still count as having been "recently" rendered.
 * @return Whether this actor was recently rendered.
 */
bool UOptimizerActorComponent::WasPrimitiveComponentRenderedRecently(UPrimitiveComponent* _PrimitiveComponent, float _Tolerance) const
{
	if (_PrimitiveComponent == nullptr)
		return false;
	if (const UWorld* const World = GetWorld())
	{
		return (World) ? (World->TimeSince(_PrimitiveComponent->GetLastRenderTimeOnScreen()) <= _Tolerance) : false;
	}
	return false;
}