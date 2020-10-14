


#include "OptimizerActorComponent.h"


#include "Runtime/Renderer/Public/PrimitiveSceneInfo.h"

// Sets default values for this component's properties
UOptimizerActorComponent::UOptimizerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("OptimizerArrow"));
    ArrowComponent->SetHiddenInGame(false,true);
	ArrowComponent->SetVisibility(true);
	ArrowComponent->ArrowColor = FColor(255, 0, 0);
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
	ArrowComponent->SetWorldLocation(GetOwner()->GetActorLocation());
	GetOwner()->AddInstanceComponent(ArrowComponent);
	
}


// Called every frame
void UOptimizerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bRenderNow = WasRecentlyRendered(RenderDelay);

	if (bCachedRenderNow != bRenderNow)
	{
		OnCheckRender.Broadcast(bRenderNow);
		bCachedRenderNow = bRenderNow;
	};
	//ArrowComponent->SetVisibility(!bRenderNow);
	// ...
}


//
bool UOptimizerActorComponent::CheckRenderInfo(float &lastTime)
{
	if (const UWorld* const World = GetWorld())
	{
		AActor* Owner = GetOwner();
		UPrimitiveComponent* PC = Cast<UPrimitiveComponent>(Owner->GetRootComponent());
		FPrimitiveSceneProxy* SP = PC->SceneProxy;
		FPrimitiveSceneInfo* PSI = SP->GetPrimitiveSceneInfo();
		lastTime = PC->GetLastRenderTime();
		return true;
	};
		//GetLastRenderTimeOnScreen(); -1 =/
	//lastTime = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->SceneProxy->GetPrimitiveSceneInfo()->LastRenderTime;
	return false;
}

bool UOptimizerActorComponent::WasRecentlyRendered(float &DelayRender, float Tolerance /*= 0.2*/) const
{
	if (const UWorld* const World = GetWorld())
	{
		// Adjust tolerance, so visibility is not affected by bad frame rate / hitches.
		const float RenderTimeThreshold = FMath::Max(Tolerance, World->DeltaTimeSeconds + KINDA_SMALL_NUMBER);

		// If the current cached value is less than the tolerance then we don't need to go look at the components
		DelayRender = World->TimeSince(GetOwner()->GetLastRenderTime());
		return DelayRender <= RenderTimeThreshold; //World->TimeSince(GetOwner()->GetLastRenderTime()) <= RenderTimeThreshold;
	}
	return false;
}

