#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
    ExplosionDelay = 3.0f;  // 오버랩 후 3초 뒤 폭발
    ExplosionRadius = 300.0f;
    ExplosionDamage = 30.f;
    ItemType = "Mine";
    bHasExploded = false;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);

    // 오버랩 이벤트 추가
    ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &AMineItem::OnItemOverlap);
}

void AMineItem::BeginPlay()
{
    Super::BeginPlay();

    // 레벨 변경 시 타이머 자동 정리
    FWorldDelegates::OnWorldCleanup.AddUObject(this, &AMineItem::OnWorldCleanup);
}

void AMineItem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
    if (World == GetWorld())
    {
        ClearTimers();
    }
}

void AMineItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bHasExploded || !OtherActor || !OtherActor->ActorHasTag("Player")) return;

    UE_LOG(LogTemp, Warning, TEXT("Mine triggered! Exploding in %f seconds"), ExplosionDelay);

    // 3초 후 폭발 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        ExplosionTimerHandle,
        this,
        &AMineItem::Explode,
        ExplosionDelay,
        false
    );
}

void AMineItem::Explode()
{
    if (!IsValid(this) || !GetWorld() || bHasExploded) return;

    bHasExploded = true;

    UParticleSystemComponent* Particle = nullptr;

    if (ExplosionParticle)
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );

        if (Particle)
        {
            Particle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            ExplosionSound,
            GetActorLocation()
        );
    }

    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // 파티클 정리
    TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
    TWeakObjectPtr<AMineItem> WeakThis = this; // 자기 자신을 TWeakObjectPtr로 저장

    if (IsValid(Particle))
    {
        FTimerHandle DestroyParticleTimerHandle;

        if (IsValid(this) && GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(
                DestroyParticleTimerHandle,
                [WeakThis, WeakParticle]()
                {
                    // 레벨이 바뀌었거나 마인이 삭제된 경우 안전하게 종료
                    if (!WeakThis.IsValid()) return;

                    if (WeakParticle.IsValid())
                    {
                        WeakParticle->DestroyComponent();
                    }

                    // 마인을 안전하게 삭제
                    WeakThis->DestroyItem();
                },
                2.0f,
                false
            );
        }
    }
}

void AMineItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ClearTimers();
    UE_LOG(LogTemp, Warning, TEXT("EndPlay called for %s"), *GetName());
    DestroyItem();
}

void AMineItem::Destroyed()
{
    Super::Destroyed();
    ClearTimers();
    UE_LOG(LogTemp, Warning, TEXT("Destroyed called for %s"), *GetName());
}

void AMineItem::ClearTimers()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }
}
