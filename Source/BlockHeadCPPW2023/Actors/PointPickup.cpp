// Fill out your copyright notice in the Description page of Project Settings.


#include "PointPickup.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter.h"
#include "BlockHeadCPPW2023/GluttonTools.h"

// Sets default values
APointPickup::APointPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(Cube);
	RootComponent = Cube;

}

// Called when the game starts or when spawned
void APointPickup::BeginPlay()
{
	Super::BeginPlay();
	if(Cube)
	{
		Cube->OnComponentBeginOverlap.AddDynamic(this, &APointPickup::OnBeginOverlap);
	}	
}

void APointPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GLUTTON_LOG("Player overlapped!");
	if(OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		Cast<APlayerCharacter>(OtherActor)->IncrementScore(PointValue);
		Destroy();
	}
}

void APointPickup::OnConstruction(const FTransform& Transform)
{
	if (Niagara)
	{
		Niagara->SetColorParameter(TEXT("Color"), PickupColor);
	}

	DynamicMaterial = Cube->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), PickupColor);
}

// Called every frame
void APointPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

