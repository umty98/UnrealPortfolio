#include "Component/C_FeetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Unit/C_Unit.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"


UC_FeetComponent::UC_FeetComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UC_FeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AC_Unit>(GetOwner());

}


// Called every frame
void UC_FeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance, rightDistance;

	FRotator leftRotation, rightRotation;

	Trace(LeftSocket,  leftDistance,  leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);

	Data.RootBoneDistance.Z = UKismetMathLibrary::FInterpTo(Data.RootBoneDistance.Z, offset, DeltaTime, (InterSpeed/30));

	Data.LeftDistance.Z  = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Z,   ( leftDistance - offset), DeltaTime, InterSpeed);
	Data.RightDistance.Z = UKismetMathLibrary::FInterpTo(Data.RightDistance.Z,  (rightDistance - offset), DeltaTime, InterSpeed);

	Data.LeftRotation  = UKismetMathLibrary::RInterpTo(Data.LeftRotation,   leftRotation, DeltaTime, InterSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterSpeed);
}

void UC_FeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	if (OwnerCharacter == nullptr)
		return;

	FVector socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float z = OwnerCharacter->GetActorLocation().Z;

	FVector start = FVector(socket.X, socket.Y, z);

	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;

	FVector end = FVector(socket.X, socket.Y, z);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;

	
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start, end,
		ETraceTypeQuery::TraceTypeQuery1, 
		true, 
		ignoreActors,
		DrawDebug,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
	

	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!hitResult.bBlockingHit)
		return;

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	OutDistance = length + OffsetDistance - TraceDistance;

//	if (length >= OffsetDistance)
//		OffsetDistance = length;

	float roll  =  UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);

	FMath::Clamp(roll,  -15, +15);
	FMath::Clamp(pitch, -15, +15);

	OutRotation = FRotator(pitch, 0,  roll);

}

