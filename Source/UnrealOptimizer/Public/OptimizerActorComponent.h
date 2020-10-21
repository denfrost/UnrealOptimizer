#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

//#include "Runtime/SlateCore/Public/Input/Reply.h"

#include "OptimizerActorComponent.generated.h"

/* Called when Render Component Render*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventDelegate_OnCheckRender, bool, RenderFlag, bool, VisibleFlag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventDelegate_OnCheckVisible, bool, VisibleFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetLastRenderTime);
typedef TBaseDelegate<void, float> FTestDelegate;
DECLARE_DELEGATE_OneParam(FTestDelegate, float); 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALOPTIMIZER_API UOptimizerActorComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	UArrowComponent* ArrowOptimizer;
	
public:	
	// Sets default values for this component's properties
	UOptimizerActorComponent();
	
	//UPROPERTY( Category = "UnrealOptimizer")
	FTestDelegate OnDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UnrealOptimizer")
	FOnSetLastRenderTime OnSetLastRenderTime;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UnrealOptimizer")
	FEventDelegate_OnCheckRender OnCheckRender;
	/* Called when Visible OnScreen Component*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UnrealOptimizer")
	FEventDelegate_OnCheckVisible OnCheckVisible;

	UPROPERTY(EditAnywhere, Category = "UnrealOptimizer")
	bool bShowActorsOptimizer;
	/** Main tick function for the Component */
    // r.VisualizeOccludedPrimitives 1
	// FreezeRendering
	UPROPERTY(EditAnywhere,Category = "UnrealOptimizer")
	UPrimitiveComponent* MonitorComponent;

	//Occlusion Property Info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnrealOptimizer")
	bool bOnVisibleNow;

	bool bCachedVisibleNow = bOnVisibleNow; // Cached for one fire evemt
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnrealOptimizer")
	float OccludedDelay = 0.0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "UnrealOptimizer", meta = (WorldContext = WorldContextObject))
	bool CheckRenderInfo(UPrimitiveComponent* _PrimitiveComp);

	/** 1 Check Render Condition
	 * My Custom. Returns true if this component has been rendered "recently", with a tolerance in seconds to define what "recent" means.
	 * e.g.: If a tolerance of 0.1 is used, this function will return true only if the actor was rendered in the last 0.1 seconds of game time.
	 *
	 * @param Tolerance  How many seconds ago the actor last render time can be and still count as having been "recently" rendered.
	 * @return Whether this actor was recently rendered.
	 */
	UFUNCTION(Category = "UnrealOptimizer", BlueprintCallable, meta = (DisplayName = "WasRecentlyRendered", Keywords = "render"))
	bool WasRecentlyRendered(float &DelayRender, float Tolerance = 0.2) const;

	/** 2 Rendering and Culling. So this function will be use for maximum efficiency optimised
	* Not forget about object visibility after selected in Pie!
	*/
	UFUNCTION(Category = "UnrealOptimizer", BlueprintCallable, meta = (DisplayName = "WasRecentlyVisible", Keywords = "visible"))
	bool WasRecentlyVisible(float Tolerance = 0.2) const;

	UFUNCTION(Category = "UnrealOptimizer", BlueprintCallable, meta = (DisplayName = "WasPrimitiveComponentRenderedRecently", Keywords = "recent"))
	bool WasPrimitiveComponentRenderedRecently(UPrimitiveComponent* _PrimitiveComponent, float _Tolerance = 0.2) const;

private:
	//Render Property Info
	bool bRenderNow;

	bool bCachedRenderNow = bRenderNow; // Cached for one fire event
		float RenderDelay = 0.0;



};




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALOPTIMIZER_API UOptimizerArrow : public UArrowComponent
{
	GENERATED_BODY()
public:

};

// Hard
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALOPTIMIZER_API UMyPrimitive : public UPrimitiveComponent
{
	GENERATED_BODY()
public:
	/* Called when Visible OnScreen Component*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UnrealOptimizer")
	FEventDelegate_OnCheckVisible OnCheckVisible;

	void SetLastRenderTime(float InLastRenderTime);
};