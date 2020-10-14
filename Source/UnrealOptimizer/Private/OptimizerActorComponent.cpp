


#include "OptimizerActorComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Runtime/Renderer/Public/PrimitiveSceneInfo.h"

// Sets default values for this component's properties
UOptimizerActorComponent::UOptimizerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITORONLY_DATA 	// ...Usefull Flag Optimizer 
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->SetHiddenInGame(false);
		ArrowComponent->ArrowColor = FColor(255, 0, 0);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetRelativeLocation(FVector(0,0,120));
		ArrowComponent->SetRelativeScale3D(FVector(2, 2, 2));
	}
#endif // WITH_EDITORONLY_DATA
}


// Called when the game starts
void UOptimizerActorComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Optimizer Activated!"));
	// ...
	
}


// Called every frame
void UOptimizerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bRenderNow = WasRecentlyRendered(RenderDelay);

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

