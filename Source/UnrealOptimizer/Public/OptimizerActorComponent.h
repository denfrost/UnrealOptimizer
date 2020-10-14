

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/ArrowComponent.h"

#include "OptimizerActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALOPTIMIZER_API UOptimizerActorComponent : public UActorComponent
{
	GENERATED_BODY()

	/** Component shown in the editor only to indicate optimizer component*/
#if WITH_EDITORONLY_DATA 
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif 
public:	
	// Sets default values for this component's properties
	UOptimizerActorComponent();
	
	/** Main tick function for the Component */
    // r.VisualizeOccludedPrimitives 1

	//Render Property Info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnrealOptimizer")
	bool bRenderNow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnrealOptimizer")
	float RenderDelay = 0.0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "UnrealOptimizer", meta = (WorldContext = WorldContextObject))
	bool CheckRenderInfo(float &lastTime);

	/**
	 * My Custom. Returns true if this component has been rendered "recently", with a tolerance in seconds to define what "recent" means.
	 * e.g.: If a tolerance of 0.1 is used, this function will return true only if the actor was rendered in the last 0.1 seconds of game time.
	 *
	 * @param Tolerance  How many seconds ago the actor last render time can be and still count as having been "recently" rendered.
	 * @return Whether this actor was recently rendered.
	 */
	UFUNCTION(Category = "UnrealOptimizer", BlueprintCallable, meta = (DisplayName = "WasComponentRecentlyRenderedOptimizer", Keywords = "scene visible"))
	bool WasRecentlyRendered(float &DelayRender,float Tolerance = 0.2) const;
};
