#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Player.h"
#include "game/act/tu_802E9428.h"
#include "game/act/tu_80362634.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_kaepora_gaebora_ending.h"


// One step of the sequence: a negative value ends the sequence and restarts it.
struct KaeporaStep {
    s32 mValue;
    s32 mCount;
};

// The flight scripts field_0x284 walks (fn_80360914): a command number
// followed by its arguments, all f32; 11 ends a script.

static const f32 lbl_80472848[] = {
    12.0f, 3030.0f, 1748.0f, 60.0f, 13.0f, 0.0f, 1.765f, 2950.0f,
    1728.0f, 60.0f, 4.0f, 1.765f, 31232.0f, 675.0f, 2.0f, 1.765f,
    34304.0f, 675.0f, 4.0f, 1.765f, 32256.0f, 675.0f, 2.0f, 1.765f,
    35328.0f, 80.0f, 2.0f, 1.765f, 32768.0f, 80.0f, 13.0f, 0.0f,
    1.765f, 2000.0f, -1000.0f, 60.0f, 11.0f,
};

static const f32 lbl_804728DC[] = {
    12.0f, 2080.0f, 100.0f, 60.0f, 13.0f, 0.0f, 1.765f, 1980.0f,
    140.0f, 60.0f, 4.0f, 1.765f, 13824.0f, 280.0f, 2.0f, 1.765f,
    18944.0f, 350.0f, 4.0f, 1.765f, 13824.0f, 350.0f, 2.0f, 1.765f,
    18944.0f, 350.0f, 4.0f, 1.765f, 13824.0f, 350.0f, 2.0f, 1.765f,
    32768.0f, 160.0f, 13.0f, 0.0f, 1.765f, 900.0f, -1000.0f, 60.0f,
    11.0f,
};

static const f32 lbl_80472980[] = {
    12.0f, 230.0f, -30.0f, 15.0f, 14.0f, 6.0f, 80.0f, 15.0f,
    0.0f, 1.765f, -1000.0f, 70.0f, 15.0f, 4.0f, 1.765f, 8192.0f,
    50.0f, 13.0f, 0.0f, 1.765f, 460.0f, 340.0f, 15.0f, 4.0f,
    1.765f, 0.0f, 50.0f, 0.0f, 1.765f, 720.0f, -1000.0f, 15.0f,
    6.0f, 100.0f, 0.0f, 1.765f, 1100.0f, -1000.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472A20[] = {
    12.0f, 480.0f, 530.0f, 15.0f, 13.0f, 0.0f, 1.765f, 512.0f,
    -1000.0f, 15.0f, 2.0f, 1.765f, 7168.0f, 200.0f, 1.0f, 1.765f,
    120.0f, 4.0f, 1.765f, 58368.0f, 180.0f, 1.0f, 1.765f, 200.0f,
    2.0f, 1.765f, 7168.0f, 200.0f, 1.0f, 1.765f, 60.0f, 4.0f,
    1.765f, 58368.0f, 150.0f, 1.0f, 1.765f, 150.0f, 11.0f,
};

static const f32 lbl_80472ABC[] = {
    12.0f, 330.0f, 3500.0f, 15.0f, 14.0f, 6.0f, 30.0f, 15.0f,
    13.0f, 0.0f, 1.765f, -1000.0f, 3312.0f, 15.0f, 6.0f, 50.0f,
    0.0f, 1.765f, -1000.0f, 2670.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472B14[] = {
    12.0f, 1024.0f, 1728.0f, 15.0f, 13.0f, 0.0f, 1.765f, 1440.0f,
    1728.0f, 15.0f, 4.0f, 1.765f, 57344.0f, 30.0f, 1.0f, 1.765f,
    40.0f, 2.0f, 1.765f, 0.0f, 30.0f, 1.0f, 1.765f, 100.0f,
    2.0f, 1.765f, 10240.0f, 100.0f, 1.0f, 1.765f, 50.0f, 4.0f,
    1.765f, 49152.0f, 70.0f, 2.0f, 1.765f, 61440.0f, 70.0f, 4.0f,
    1.765f, 45056.0f, 100.0f, 2.0f, 1.765f, 53248.0f, 500.0f, 4.0f,
    1.765f, 45056.0f, 500.0f, 2.0f, 1.765f, 53248.0f, 500.0f, 4.0f,
    1.765f, 45056.0f, 500.0f, 11.0f,
};

static const f32 lbl_80472C04[] = {
    12.0f, 1918.0f, 400.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    356.0f, 15.0f, 2.0f, 1.765f, 0.0f, 66.0f, 0.0f, 1.765f,
    3088.0f, -1000.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472C54[] = {
    12.0f, 720.0f, 3450.0f, 15.0f, 14.0f, 6.0f, 40.0f, 15.0f,
    0.0f, 1.765f, -1000.0f, 3280.0f, 15.0f, 2.0f, 1.765f, 61440.0f,
    30.0f, 13.0f, 0.0f, 1.765f, 980.0f, 3230.0f, 15.0f, 2.0f,
    1.8f, 0.0f, 100.0f, 0.0f, 1.9f, 1200.0f, -1000.0f, 15.0f,
    0.0f, 2.0f, 1310.0f, -1000.0f, 15.0f, 0.0f, 1.8f, 1420.0f,
    -1000.0f, 15.0f, 0.0f, 1.6f, 1530.0f, -1000.0f, 15.0f, 0.0f,
    1.3f, 1900.0f, -1000.0f, 15.0f, 6.0f, 80.0f, 0.0f, 2.5f,
    1899.0f, -1000.0f, 15.0f, 4.0f, 2.5f, 16384.0f, 5.0f, 4.0f,
    2.75f, 0.0f, 7.5f, 4.0f, 3.0f, 49152.0f, 10.5f, 4.0f,
    3.25f, 32768.0f, 12.0f, 4.0f, 3.25f, 16384.0f, 14.0f, 4.0f,
    3.25f, 49152.0f, 16.0f, 5.0f, 3.25f, 65536.0f, 16.0f, 0.0f,
    2.25f, -1000.0f, 3000.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472DC8[] = {
    12.0f, 1744.0f, 2600.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    2270.0f, 15.0f, 2.0f, 1.765f, 0.0f, 25.0f, 4.0f, 1.765f,
    49152.0f, 32.0f, 0.0f, 1.765f, -1000.0f, 1716.0f, 15.0f, 2.0f,
    1.765f, 0.0f, 50.0f, 13.0f, 0.0f, 1.765f, 2100.0f, -1000.0f,
    15.0f, 11.0f,
};

static const f32 lbl_80472E50[] = {
    12.0f, 768.0f, 3000.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    1900.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472E7C[] = {
    12.0f, 768.0f, 780.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    200.0f, 15.0f, 6.0f, 120.0f, 10.0f, 11.0f,
};

static const f32 lbl_80472EB4[] = {
    12.0f, 768.0f, 1880.0f, 15.0f, 14.0f, 6.0f, 40.0f, 15.0f,
    0.0f, 1.765f, -1000.0f, 1680.0f, 15.0f, 4.0f, 1.765f, 24576.0f,
    30.0f, 4.0f, 1.765f, 28672.0f, 55.0f, 4.0f, 1.765f, 34816.0f,
    70.0f, 6.0f, 60.0f, 13.0f, 0.0f, 1.765f, -1000.0f, 1402.0f,
    15.0f, 2.0f, 1.765f, 0.0f, 50.0f, 0.0f, 1.765f, 1240.0f,
    -1000.0f, 15.0f, 4.0f, 1.765f, 49152.0f, 40.0f, 0.0f, 1.765f,
    -1000.0f, 1100.0f, 15.0f, 11.0f,
};

static const f32 lbl_80472F84[] = {
    12.0f, 768.0f, 2310.0f, 15.0f, 14.0f, 6.0f, 80.0f, 15.0f,
    0.0f, 1.765f, -1000.0f, 2190.0f, 15.0f, 4.0f, 1.765f, 40960.0f,
    20.0f, 13.0f, 1.0f, 1.765f, 36.0f, 2.0f, 1.765f, 49152.0f,
    30.0f, 0.0f, 1.765f, -1000.0f, 1806.0f, 15.0f, 2.0f, 1.765f,
    57344.0f, 30.0f, 1.0f, 1.765f, 126.0f, 4.0f, 1.765f, 49152.0f,
    50.0f, 0.0f, 1.765f, -1000.0f, 1500.0f, 15.0f, 11.0f,
};

static const f32 lbl_80473040[] = {
    12.0f, 1500.0f, 1344.0f, 15.0f, 13.0f, 0.0f, 1.765f, 1748.0f,
    -1000.0f, 15.0f, 4.0f, 1.765f, 49152.0f, 30.0f, 0.0f, 1.765f,
    -1000.0f, 1100.0f, 15.0f, 4.0f, 1.765f, 32768.0f, 115.0f, 13.0f,
    0.0f, 1.765f, 1020.0f, 960.0f, 15.0f, 11.0f,
};

static const f32 lbl_804730B8[] = {
    12.0f, 1280.0f, 3350.0f, 15.0f, 14.0f, 6.0f, 80.0f, 15.0f,
    13.0f, 0.0f, 1.765f, -1000.0f, 3036.0f, 15.0f, 2.0f, 1.765f,
    59392.0f, 32.0f, 1.0f, 1.765f, 200.0f, 2.0f, 1.765f, 0.0f,
    32.0f, 0.0f, 1.765f, 1808.0f, -1000.0f, 15.0f, 4.0f, 1.765f,
    49152.0f, 50.0f, 0.0f, 1.765f, -1000.0f, 2430.0f, 15.0f, 6.0f,
    120.0f, 13.0f, 10.0f, 11.0f,
};

static const f32 lbl_80473168[] = {
    12.0f, 3030.0f, 1728.0f, 60.0f, 13.0f, 0.0f, 1.765f, 2320.0f,
    -1000.0f, 60.0f, 3.0f, 1.32375f, 65536.0f, 60.0f, 13.0f, 0.0f,
    1.765f, 2000.0f, -1000.0f, 60.0f, 11.0f,
};

static const f32 lbl_804731BC[] = {
    12.0f, 2080.0f, 100.0f, 60.0f, 13.0f, 0.0f, 1.765f, 1980.0f,
    140.0f, 60.0f, 4.0f, 1.765f, 16384.0f, 280.0f, 0.0f, 1.7f,
    -1000.0f, 1100.0f, 60.0f, 2.0f, 1.765f, 32768.0f, 250.0f, 13.0f,
    0.0f, 1.765f, 900.0f, -1000.0f, 60.0f, 11.0f,
};

static const f32 lbl_80473234[] = {
    12.0f, 230.0f, -30.0f, 15.0f, 6.0f, 80.0f, 0.0f, 1.765f,
    -1000.0f, 70.0f, 15.0f, 4.0f, 1.765f, 8192.0f, 50.0f, 13.0f,
    0.0f, 1.765f, 460.0f, 340.0f, 15.0f, 4.0f, 1.765f, 0.0f,
    50.0f, 0.0f, 1.765f, 720.0f, -1000.0f, 15.0f, 6.0f, 100.0f,
    0.0f, 1.765f, 1100.0f, -1000.0f, 15.0f, 11.0f,
};

static const f32 lbl_804732CC[] = {
    12.0f, 480.0f, 530.0f, 15.0f, 13.0f, 0.0f, 1.765f, 512.0f,
    -1000.0f, 15.0f, 2.0f, 1.765f, 7168.0f, 200.0f, 1.0f, 1.765f,
    120.0f, 4.0f, 1.765f, 58368.0f, 180.0f, 1.0f, 1.765f, 200.0f,
    2.0f, 1.765f, 7168.0f, 200.0f, 1.0f, 1.765f, 60.0f, 4.0f,
    1.765f, 58368.0f, 150.0f, 1.0f, 1.765f, 150.0f, 11.0f,
};

static const f32 lbl_80473368[] = {
    12.0f, 330.0f, 3500.0f, 15.0f, 6.0f, 30.0f, 13.0f, 0.0f,
    1.765f, -1000.0f, 3312.0f, 15.0f, 6.0f, 50.0f, 0.0f, 1.765f,
    -1000.0f, 2670.0f, 15.0f, 11.0f,
};

static const f32 lbl_804733B8[] = {
    12.0f, 1024.0f, 1728.0f, 15.0f, 13.0f, 0.0f, 1.765f, 1630.0f,
    1728.0f, 15.0f, 2.0f, 1.765f, 10240.0f, 100.0f, 1.0f, 1.765f,
    50.0f, 4.0f, 1.765f, 49152.0f, 70.0f, 2.0f, 1.765f, 61440.0f,
    70.0f, 4.0f, 1.765f, 45056.0f, 100.0f, 2.0f, 1.765f, 53248.0f,
    500.0f, 4.0f, 1.765f, 45056.0f, 500.0f, 2.0f, 1.765f, 53248.0f,
    500.0f, 4.0f, 1.765f, 45056.0f, 500.0f, 11.0f,
};

static const f32 lbl_80473470[] = {
    12.0f, 1900.0f, 400.0f, 15.0f, 13.0f, 0.0f, 1.8f, -1000.0f,
    356.0f, 15.0f, 2.0f, 1.765f, 0.0f, 66.0f, 0.0f, 1.765f,
    3088.0f, -1000.0f, 15.0f, 11.0f,
};

static const f32 lbl_804734C0[] = {
    12.0f, 720.0f, 3450.0f, 15.0f, 6.0f, 40.0f, 0.0f, 1.765f,
    -1000.0f, 3280.0f, 15.0f, 2.0f, 1.765f, 61440.0f, 30.0f, 13.0f,
    0.0f, 1.765f, 980.0f, 3230.0f, 15.0f, 2.0f, 1.8f, 0.0f,
    100.0f, 0.0f, 1.9f, 1200.0f, -1000.0f, 15.0f, 0.0f, 2.0f,
    1330.0f, -1000.0f, 15.0f, 0.0f, 1.8f, 1450.0f, -1000.0f, 15.0f,
    0.0f, 1.6f, 1560.0f, -1000.0f, 15.0f, 0.0f, 1.3f, 1900.0f,
    -1000.0f, 15.0f, 6.0f, 160.0f, 13.0f, 6.0f, 100.0f, 11.0f,
};

static const f32 lbl_804735A0[] = {
    12.0f, 1744.0f, 2600.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    2265.0f, 15.0f, 0.0f, 1.7f, 1792.0f, 1716.0f, 15.0f, 2.0f,
    1.765f, 0.0f, 50.0f, 13.0f, 0.0f, 1.765f, 2100.0f, -1000.0f,
    15.0f, 11.0f,
};

static const f32 lbl_80473608[] = {
    12.0f, 768.0f, 3000.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    1900.0f, 15.0f, 11.0f,
};

static const f32 lbl_80473634[] = {
    12.0f, 768.0f, 780.0f, 15.0f, 13.0f, 0.0f, 1.765f, -1000.0f,
    200.0f, 15.0f, 6.0f, 120.0f, 10.0f, 11.0f,
};

static const f32 lbl_8047366C[] = {
    12.0f, 768.0f, 1880.0f, 15.0f, 6.0f, 40.0f, 0.0f, 1.765f,
    -1000.0f, 1680.0f, 15.0f, 4.0f, 1.765f, 24576.0f, 30.0f, 4.0f,
    1.765f, 28672.0f, 55.0f, 4.0f, 1.765f, 34816.0f, 70.0f, 6.0f,
    60.0f, 13.0f, 0.0f, 1.765f, -1000.0f, 1402.0f, 15.0f, 2.0f,
    1.765f, 0.0f, 50.0f, 0.0f, 1.765f, 1240.0f, -1000.0f, 15.0f,
    4.0f, 1.765f, 49152.0f, 40.0f, 0.0f, 1.765f, -1000.0f, 1100.0f,
    15.0f, 11.0f,
};

static const f32 lbl_80473734[] = {
    12.0f, 768.0f, 2310.0f, 15.0f, 6.0f, 80.0f, 0.0f, 1.765f,
    -1000.0f, 2190.0f, 15.0f, 4.0f, 1.765f, 40960.0f, 20.0f, 13.0f,
    1.0f, 1.765f, 36.0f, 2.0f, 1.765f, 49152.0f, 30.0f, 0.0f,
    1.765f, -1000.0f, 1806.0f, 15.0f, 2.0f, 1.765f, 57344.0f, 30.0f,
    1.0f, 1.765f, 126.0f, 4.0f, 1.765f, 49152.0f, 50.0f, 0.0f,
    1.765f, -1000.0f, 1500.0f, 15.0f, 11.0f,
};

static const f32 lbl_804737E8[] = {
    12.0f, 1500.0f, 1344.0f, 15.0f, 13.0f, 0.0f, 1.765f, 1748.0f,
    -1000.0f, 15.0f, 4.0f, 1.765f, 49152.0f, 30.0f, 0.0f, 1.765f,
    -1000.0f, 1100.0f, 15.0f, 4.0f, 1.765f, 32768.0f, 115.0f, 13.0f,
    0.0f, 1.765f, 1020.0f, 960.0f, 15.0f, 11.0f,
};

static const f32 lbl_80473860[] = {
    12.0f, 1280.0f, 3350.0f, 15.0f, 6.0f, 80.0f, 13.0f, 0.0f,
    1.765f, -1000.0f, 3036.0f, 15.0f, 2.0f, 1.765f, 59392.0f, 32.0f,
    1.0f, 1.765f, 200.0f, 2.0f, 1.765f, 0.0f, 32.0f, 0.0f,
    1.765f, 1808.0f, -1000.0f, 15.0f, 4.0f, 1.765f, 49152.0f, 50.0f,
    0.0f, 1.765f, -1000.0f, 2430.0f, 15.0f, 6.0f, 80.0f, 13.0f,
    6.0f, 200.0f, 11.0f,
};

static const f32 lbl_8047390C[] = {
    12.0f, 440.0f, 3312.0f, 16.0f, 6.0f, 10000.0f, 11.0f,
};

static const f32 lbl_80473928[] = {
    12.0f, 1824.0f, 1792.0f, 60.0f, 14.0f, 6.0f, 666.0f, 12.0f,
    1700.0f, 1496.0f, 60.0f, 15.0f, 0.0f, 3.2f, 2100.0f, -1000.0f,
    60.0f, 4.0f, 7.0f, 32768.0f, 100.0f, 0.0f, 3.2f, 1700.0f,
    -1000.0f, 60.0f, 11.0f,
};

static const f32 lbl_80473994[] = {
    12.0f, 1988.0f, 500.0f, 60.0f, 6.0f, 130.0f, 12.0f, 1878.0f,
    256.0f, 60.0f, 0.0f, 4.25f, 2500.0f, -1000.0f, 60.0f, 11.0f,
};

static const f32 lbl_804739D4[] = {
    12.0f, 768.0f, 3700.0f, 0.0f, 14.0f, 13.0f, 6.0f, 100000.0f,
    11.0f,
};

static const f32 lbl_804739F8[] = {
    12.0f, 704.0f, 3704.0f, 0.0f, 14.0f, 13.0f, 6.0f, 100000.0f,
    11.0f,
};

static const f32 lbl_80473A1C[] = {
    12.0f, 2604.0f, 130.0f, 64.0f, 6.0f, 100000.0f, 11.0f,
};

// The paths field_0x298 follows; mKind -1000 ends a path.

static const KaeporaWaypoint lbl_80473A38[] = {
    {-1, -2.0f, 350.0f, 1.0f, 500.0f, 1},
    {-4, -1.0f, 2730.0f, 0.0f, 0.0f, 0},
    {-2, -2.0f, 2100.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473A98[] = {
    {-1, -2.0f, 350.0f, 1.0f, 500.0f, 1},
    {-4, -4.0f, 0.0f, 130.0f, 0.0f, 0},
    {-3, -1.0f, 1480.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473AF8[] = {
    {-4, -2.0f, 120.0f, 0.0f, 0.0f, 1},
    {-2, -2.0f, 950.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473B40[] = {
    {-4, -2.0f, 580.0f, 0.0f, 0.0f, 1},
    {-3, -2.0f, 1400.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473B88[] = {
    {-4, -3.0f, 0.0f, 3270.0f, 0.0f, 1},
    {-2, -3.0f, 0.0f, 2820.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473BD0[] = {
    {-4, -2.0f, 1065.0f, 0.0f, 0.0f, 1},
    {-2, -3.0f, 0.0f, 1190.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473C18[] = {
    {-4, -3.0f, 0.0f, 370.0f, 0.0f, 1},
    {-1, -2.0f, 500.0f, 120.0f, 2388.0f, 1},
    {-3, -2.0f, 2988.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473C78[] = {
    {-4, -2.0f, 780.0f, 0.0f, 0.0f, 1},
    {2, -2.0f, 920.0f, 0.0f, 0.0f, 1},
    {1, -2.0f, 1100.0f, 0.0f, 0.0f, 0},
    {-2, -2.0f, 0.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473CF0[] = {
    {-4, -3.0f, 0.0f, 2520.0f, 0.0f, 0},
    {-2, -2.0f, 0.0f, 2000.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473D38[] = {
    {-4, -3.0f, 0.0f, 2850.0f, 0.0f, 1},
    {-2, -3.0f, 0.0f, 1960.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473D80[] = {
    {-4, -3.0f, 0.0f, 700.0f, 0.0f, 1},
    {-3, -3.0f, 0.0f, 205.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473DC8[] = {
    {-4, -3.0f, 0.0f, 1650.0f, 0.0f, 1},
    {-3, -2.0f, 1260.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473E10[] = {
    {-4, -3.0f, 0.0f, 2200.0f, 0.0f, 1},
    {-2, -3.0f, 0.0f, 1550.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473E58[] = {
    {-4, -2.0f, 1580.0f, 0.0f, 0.0f, 0},
    {-3, -1.0f, 1280.0f, 0.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473EA0[] = {
    {-4, -3.0f, 0.0f, 3200.0f, 0.0f, 0},
    {-3, -3.0f, 0.0f, 2440.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473EE8[] = {
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473F00[] = {
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473F18[] = {
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473F30[] = {
    {-1, -4.0f, 290.0f, 1.0f, 0.0f, 1},
    {-4, -3.0f, 0.0f, 3700.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473F78[] = {
    {-4, -2.0f, 0.0f, 0.0f, 0.0f, 1},
    {-1, -2.0f, 300.0f, 1.0f, 0.0f, 0},
    {-4, -3.0f, 0.0f, 3800.0f, 0.0f, 0},
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473FD8[] = {
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

static const KaeporaWaypoint lbl_80473FF0[] = {
    {-1000, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

// Sprite frame base per field_0x270 - 1 (init).
static const s32 lbl_80474008[] = {470, 488, 464, 494, 482, 476};

// Head directions, indexed by field_0x274.
static const s32 lbl_80474020[4] = {0, 2, 4, 6};

// The head step tables (lbl_804ADA98); {-1, -1} ends one.

static const KaeporaStep lbl_80474030[] = {
    {0, 10}, {1, 10}, {-1, -1},
};

static const KaeporaStep lbl_80474048[] = {
    {4, 15}, {0, 4}, {4, 2}, {2, 8}, {3, 6}, {-1, -1},
};

static const KaeporaStep lbl_80474078[] = {
    {5, 10}, {6, 10}, {-1, -1},
};

static const KaeporaStep lbl_80474090[] = {
    {9, 15}, {5, 4}, {9, 2}, {7, 8}, {8, 6}, {-1, -1},
};

static const KaeporaStep lbl_804740C0[] = {
    {10, 10}, {11, 10}, {-1, -1},
};

static const KaeporaStep lbl_804740D8[] = {
    {14, 15}, {10, 4}, {14, 2}, {12, 8}, {13, 6}, {-1, -1},
};

static const KaeporaStep lbl_80474108[] = {
    {15, 10}, {16, 10}, {-1, -1},
};

static const KaeporaStep lbl_80474120[] = {
    {19, 15}, {15, 4}, {19, 2}, {17, 8}, {18, 6}, {-1, -1},
};

static const KaeporaStep lbl_80474150[] = {
    {6, 4}, {7, 4}, {8, 7}, {9, 5}, {10, 4}, {11, 5}, {12, 4}, {13, 4}, {14, 10}, {-1, -1},
};

static const KaeporaStep lbl_804741A0[] = {
    {21, 4}, {22, 4}, {23, 7}, {24, 5}, {25, 4}, {26, 5}, {27, 4}, {28, 4}, {14, 10}, {-1, -1},
};

static const KaeporaStep lbl_804741F0[] = {
    {14, 10}, {13, 4}, {12, 4}, {11, 5}, {10, 4}, {9, 5}, {8, 7}, {7, 4}, {6, 4}, {-1, -1},
};

static const KaeporaStep lbl_80474240[] = {
    {14, 10}, {28, 4}, {27, 4}, {26, 5}, {25, 4}, {24, 5}, {23, 7}, {22, 4}, {21, 4}, {-1, -1},
};

// 1: spawn an EOWL partner, indexed by field_0x250.
static const u8 lbl_80474290[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0};


// The head sway targets (mStep).
static const KaeporaStep lbl_804742A8[] = {
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
    {-1, -1},
};

// The colour of each partner's glow (emitter env colour), by field_0x270 - 1.
static const u8 lbl_804742E0[][3] = {
    {0x2D, 0x78, 0xFF}, {0xC8, 0xA0, 0x28}, {0x4B, 0xFF, 0x2D}, {0x1E, 0xA0, 0xBE}, {0xFF, 0x41, 0x37}, {0xDC, 0x1E, 0xDC},
};

// One-command scripts (just the end command) small enough for .sdata2.
static const f32 lbl_805429D8[] = {11.0f};
static const f32 lbl_805429DC[] = {11.0f};
static const f32 lbl_805429E0[] = {11.0f};
static const f32 lbl_805429E4[] = {11.0f};
static const f32 lbl_805429E8[] = {11.0f};
static const f32 lbl_805429EC[] = {11.0f};
static const f32 lbl_805429F0[] = {11.0f};
static const f32 lbl_805429F4[] = {11.0f};

// Facing offsets of the sprite, indexed by field_0x274 (fn_80361424).
static const u16 lbl_805429F8[4] = {0, 0x8000, 0xC000, 0x4000};

// Scripts by field_0x250 for the partner owl (flag 0x100 set).
static const f32* lbl_804AD900[22] = {
    lbl_80472848, lbl_804728DC, lbl_80472980, lbl_80472A20,
    lbl_80472ABC, lbl_80472B14, lbl_80472C04, lbl_80472C54,
    lbl_80472DC8, lbl_80472E50, lbl_80472E7C, lbl_80472EB4,
    lbl_80472F84, lbl_80473040, lbl_804730B8, lbl_805429D8,
    lbl_805429DC, lbl_805429E0, lbl_805429E4, lbl_805429E8,
    lbl_805429EC, lbl_805429F0,
};

// Scripts by field_0x250.
static const f32* lbl_804AD958[22] = {
    lbl_80473168, lbl_804731BC, lbl_80473234, lbl_804732CC,
    lbl_80473368, lbl_804733B8, lbl_80473470, lbl_804734C0,
    lbl_804735A0, lbl_80473608, lbl_80473634, lbl_8047366C,
    lbl_80473734, lbl_804737E8, lbl_80473860, lbl_8047390C,
    lbl_80473928, lbl_80473994, lbl_804739D4, lbl_804739F8,
    lbl_80473A1C, lbl_805429F4,
};

// Paths by field_0x250.
static const KaeporaWaypoint* lbl_804AD9B0[22] = {
    lbl_80473A38, lbl_80473A98, lbl_80473AF8, lbl_80473B40,
    lbl_80473B88, lbl_80473BD0, lbl_80473C18, lbl_80473C78,
    lbl_80473CF0, lbl_80473D38, lbl_80473D80, lbl_80473DC8,
    lbl_80473E10, lbl_80473E58, lbl_80473EA0, lbl_80473EE8,
    lbl_80473F00, lbl_80473F18, lbl_80473F30, lbl_80473F78,
    lbl_80473FD8, lbl_80473FF0,
};

// The update() steps, indexed by mPtmfIndex; built at static init time.
void (KaeporaGaeboraEnding_804ADAC8::*lbl_804ADA50[6])() = {
    &KaeporaGaeboraEnding_804ADAC8::fn_8036003C, &KaeporaGaeboraEnding_804ADAC8::fn_8035FF70, &KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C,
    &KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8, &KaeporaGaeboraEnding_804ADAC8::fn_8035FB64, &KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8,
};

// The head step tables, indexed by field_0x244.
static const KaeporaStep* lbl_804ADA98[12] = {
    lbl_80474030, lbl_80474048, lbl_80474078, lbl_80474090,
    lbl_804740C0, lbl_804740D8, lbl_80474108, lbl_80474120,
    lbl_80474150, lbl_804741A0, lbl_804741F0, lbl_80474240,
};

// FIXME(shotgun): 84.2 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::init() {
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23C = 0;
    mStep = 0;
    field_0x248 = 0;
    field_0x244 = 0;
    field_0x254 = 0;
    field_0x258 = field_0x25C = 0.0f;
    field_0x260 = field_0x264 = 0;
    field_0x268 = 0.0f;
    field_0x26C = 0.0f;
    field_0x270 = field_0x90 & 7;
    field_0x274 = 0;
    field_0x250 = (field_0x90 >> 3) & 0x1F;
    field_0x2B0 = 1;
    field_0x280 = 0;
    field_0x288 = field_0x28C = field_0x290 = 0.0f;
    field_0x294 = 0;
    field_0x2B4 = NULL;
    field_0x2B8 = NULL;
    field_0x29C = NULL;
    if (!(field_0x90 & 0x100)) {
        if (lbl_80474290[field_0x250] == 1) {
            field_0x2B0 = 0;
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EOWL, field_0x4, &mPos, field_0x90 | 0x100, -1, -1);
            field_0x298 = lbl_804AD9B0[field_0x250];
            field_0x284 = lbl_804AD958[field_0x250];
        } else {
            field_0x298 = lbl_804AD9B0[field_0x250];
            field_0x284 = lbl_804AD958[field_0x250];
        }
    } else {
        field_0x298 = lbl_80473FD8;
        field_0x284 = lbl_804AD900[field_0x250];
    }
    mHeight = 1;
    mDepth = 1;
    if (field_0x270 == 0) {
        fn_80360914();
    } else {
        field_0x238 = lbl_80474008[field_0x270 - 1];
        fn_80360914();
        if (field_0x90 & 0x100) {
            field_0x2B4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, mHeight, mDepth, NULL, NULL, true, 1.0f);
            field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x171, mHeight, mDepth, NULL, NULL, true, 1.0f);
            if (field_0x2B4 != NULL) {
                field_0x2B4->setGlobalEnvColor(lbl_804742E0[field_0x270 - 1][0], lbl_804742E0[field_0x270 - 1][1],
                                               lbl_804742E0[field_0x270 - 1][2]);
            }
        }
    }
    if (!(field_0x90 & 0x100)) {
        if (field_0x250 == 0x12) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_ELK2, field_0x4, &mPos, 0, -1, -1);
            static JGeometry::TVec3f offsets[4] = {
                JGeometry::TVec3f(-18.0f, -126.0f, 0.0f),
                JGeometry::TVec3f(18.0f, -126.0f, 0.0f),
                JGeometry::TVec3f(-26.0f, -156.0f, 0.0f),
                JGeometry::TVec3f(26.0f, -156.0f, 0.0f),
            };
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = mPos + offsets[i];
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EETC, field_0x4, &pos,
                                                                (((i + 0x26) << 5) | 4) | 0x80000000, -1, -1);
            }
        } else if (field_0x250 == 0x13) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EETC, field_0x4, &mPos, 0x10, -1, -1);
            field_0x230 = 1;
            JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
            fn_80230498(0, &zero, field_0x238 + field_0x23C, 0xFF, -0x80, -0x80, true);
        }
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_ENDM && actor->mAlive == true) {
                field_0x29C = (UnknownENDM_804AA0FC*)actor;
                break;
            }
        }
    }
}

void KaeporaGaeboraEnding_804ADAC8::update() {
    if (field_0x280 != 0) {
        field_0x280--;
    }
    if (field_0x234 != 0) {
        field_0x234--;
    }
    while (fn_8035F3C0()) {
    }
    (this->*lbl_804ADA50[mPtmfIndex])();
    if (field_0x2B0 == 0) {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, -2, -2, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, -2, -2, true);
        }
    } else {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, mHeight, mDepth, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, mHeight, mDepth, true);
        }
    }
}

// FIXME(shotgun): 94.2 %, different
void KaeporaGaeboraEnding_804ADAC8::draw() {
    if (field_0x2B0 != 0) {
        if (field_0x270 == 0) {
            if (mPtmfIndex == 1) {
                fn_803616FC();
            } else if (mPtmfIndex == 0) {
                fn_80361424();
            } else {
                SpriteParams_801F5AE0 params;
                params.field_0x1f = 0;
                drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x25C, &params, -1, 1.0f, 1.0f);
                params.field_0x10.a = 200;
                if (mSpriteParams.mMtx == NULL) {
                    fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", field_0x23C + 0x25C, NULL);
                }
            }
        } else {
            SpriteParams_801F5AE0 params;
            params.field_0x1f = 0;
            params.field_0x1c = mHeight;
            params.field_0x1d = mDepth;
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x238 + field_0x23C, &params, -1, 1.0f, 1.0f);
            if (mHeight < 0) {
                params.field_0x1c = 0;
            }
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_both2_sob.bin", field_0x238 + field_0x23C, NULL);
            }
        }
    }
}

// FIXME(shotgun): 90.7 %, scheduling+regalloc
void KaeporaGaeboraEnding_804ADAC8::fn_803616FC() {
    SpriteParams_801F5AE0 params;
    params.field_0x1f = 0;
    JGeometry::TVec3f savedPos = mPos;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x279, &params, -1, 1.0f, 1.0f);
    params.field_0x10.a = 200;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", 0x279, NULL);
    }
    params.reset();
    params.field_0x1f = 0;
    params.field_0x1d = 1;
    Mtx mtx;
    f32 s = JMath::sincosTable_.sinShort(mSway.mAngle);
    f32 c = JMath::sincosTable_.cosShort(mSway.mAngle);
    mtx[0][0] = c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = s;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = -s;
    mtx[2][1] = 0.0f;
    mtx[2][2] = c;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    mPos.y += -21.0f;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x27A, &params, -1, 1.0f, 1.0f);
    mPos.y -= -21.0f;
    params.field_0x10.a = 200;
    mPos.y += -10.5f;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", 0x27A, NULL);
    }
    mPos.y -= -10.5f;
    mPos = savedPos;
}

// FIXME(shotgun): 96.3 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_80361424() {
    SpriteParams_801F5AE0 params;
    bool shadow = false;
    if (field_0x250 == 1) {
        if (mPos.y >= 580.0f && mPos.y < 740.0f) {
            params.field_0x1c = 0;
            shadow = false;
            params.field_0x1d = 1;
        } else if (mPos.y >= 740.0f && mPos.y < 952.0f) {
            params.field_0x1c = 1;
            shadow = true;
            params.field_0x1d = 2;
        } else {
            params.field_0x1c = 1;
            shadow = false;
            params.field_0x1d = 2;
        }
    } else if (field_0x250 == 0x10) {
        params.field_0x1d = 2;
    } else if (field_0x250 == 0) {
        params.field_0x1c = 1;
        params.field_0x1d = 2;
    }
    params.field_0x1f = 0;
    Mtx mtx;
    s16 angle = field_0x254 - lbl_805429F8[field_0x274];
    f32 s = JMath::sincosTable_.sinShort(angle);
    f32 c = JMath::sincosTable_.cosShort(angle);
    mtx[0][0] = c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = s;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = -s;
    mtx[2][1] = 0.0f;
    mtx[2][2] = c;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    if (!shadow) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x3E0, &params, 7, 1.0f, 1.0f);
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x3E0, &params, -1, 1.0f, 1.0f);
        params.field_0x1c = 0;
        params.field_0x1d = -1;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", field_0x23C + 0x3E0, NULL);
        }
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_803613D0() {
    if (field_0x2B4 != NULL) {
        field_0x2B4->becomeInvalidEmitter();
        field_0x2B4 = NULL;
    }
    if (field_0x2B8 != NULL) {
        field_0x2B8->becomeInvalidEmitter();
        field_0x2B8 = NULL;
    }
}


// Counts the current step down and loads the next one from the table
// lbl_804ADA98[field_0x244]; true at the table's terminator.
inline bool KaeporaGaeboraEnding_804ADAC8::advanceStep() {
    s32 idx = field_0x244;
    if (field_0x234 == 0) {
        const KaeporaStep* table = lbl_804ADA98[idx];
        if (table[mStep].mValue < 0) {
            return true;
        }
        field_0x234 = table[mStep].mCount;
        field_0x23C = table[mStep].mValue;
        mStep++;
    }
    return false;
}

// The facing (0-3) of the velocity field_0x3c (fn_80360914, fn_8036003C).
inline void KaeporaGaeboraEnding_804ADAC8::updateFacing() {
    u16 a = (s32)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
    if (a >= 0x2000 && a < 0x6000) {
        field_0x274 = 3;
    } else if (a >= 0x6000 && a < 0xa000) {
        field_0x274 = 1;
    } else if (a >= 0xa000 && a < 0xe000) {
        field_0x274 = 2;
    } else {
        field_0x274 = 0;
    }
}

// Picks the flap step table for the new facing (fn_80360914, fn_8036003C).
inline void KaeporaGaeboraEnding_804ADAC8::pickStepTable() {
    if ((field_0x248 & 1) == 1 || fn_801CD664(100) < 5 ||
        (field_0x250 == 0 && field_0x27C != 0 && field_0x27C != 1 && field_0x26C < 100.0f)) {
        field_0x244++;
        if (!(field_0x248 & 1)) {
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
            mStep = 4;
            field_0x234 = 0;
            advanceStep();
            mStep = 0;
            field_0x234 = 6;
        } else {
            const KaeporaStep* table = lbl_804ADA98[field_0x244];
            if (mStep == 0) {
                field_0x23C = table[4].mValue;
            } else {
                field_0x23C = table[mStep - 1].mValue;
            }
        }
    } else {
        field_0x234 = 0;
        mStep = 0;
        advanceStep();
    }
}

// FIXME(shotgun): 0 % in objdiff (689 vs 687 instrs: our shared tail of commands 0-5 sits after the switch, the original has it right after case 0), missing/extra
// The original keeps the shared tail inside case 0 and commands 1-5 branch
// back into it; a goto layout reproduces that at 98.33 %, but goto is
// crack-pass only (docs/CRACK_PASS.md), so the tail stays after the switch.
// Reads the next command of the script field_0x284 and starts it.
void KaeporaGaeboraEnding_804ADAC8::fn_80360914() {
    field_0x26C = 0.0f;
    field_0x264 = 0;
    field_0x25C = 0.0f;
    if (field_0x270 != 0 && field_0x27C == 6 && field_0x2B8 != NULL) {
        field_0x2B8->becomeInvalidEmitter();
        field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x171, mHeight, mDepth, NULL, NULL, true, 1.0f);
    }
    field_0x27C = (s32)*field_0x284++;
    switch (field_0x27C) {
    case 0:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x288 = *field_0x284++;
        if (-1000.0f == field_0x288) {
            field_0x288 = mPos.x;
        }
        field_0x28C = *field_0x284++;
        if (-1000.0f == field_0x28C) {
            field_0x28C = mPos.y;
        }
        field_0x290 = *field_0x284++;
        aimAtTarget();
        break;
    case 1:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x258 = *field_0x284++;
        break;
    case 2:
    case 4:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x260 = (s32)*field_0x284++;
        if (field_0x27C == 2) {
            u16 a = field_0x254;
            if (a <= (u16)field_0x260) {
                field_0x260 = field_0x260 - a;
            } else {
                field_0x260 = 0x10000 - a + field_0x260;
            }
        } else {
            u16 a = field_0x254;
            if (a >= (u16)field_0x260) {
                field_0x260 = a - field_0x260;
            } else {
                field_0x260 = 0x10000 - field_0x260 + a;
            }
        }
        field_0x26C = *field_0x284++;
        break;
    case 3:
    case 5:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x260 = (s32)*field_0x284++;
        field_0x26C = *field_0x284++;
        break;
    case 6:
        mPtmfIndex = 1;
        field_0x280 = (s32)*field_0x284++;
        if (field_0x270 == 0) {
            field_0x234 = 0;
            mStep = 0;
            mSway.fn_8021ABC0();
        } else if (field_0x2B8 != NULL) {
            field_0x2B8->becomeInvalidEmitter();
            field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, mHeight, mDepth, NULL, NULL, true, 1.0f);
        }
        field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
        field_0x254 = 0;
        return;
    case 7:
        if (field_0x270 != 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 3;
            field_0x244 = fn_801CD664(2) + 8;
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
        }
        return;
    case 8:
        if (field_0x270 != 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 2;
            field_0x268 = *field_0x284++;
            field_0x288 = *field_0x284++;
            field_0x28C = *field_0x284++;
            field_0x290 = *field_0x284++;
            field_0x244 = fn_801CD664(2) + 10;
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
        }
        return;
    case 9:
        if (field_0x270 == 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 4;
            field_0x26C = 3.0f;
        }
        return;
    case 10:
        if (field_0x270 == 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 5;
            field_0x26C = 15.0f;
        }
        return;
    case 11:
        fn_803613D0();
        mAlive = false;
        return;
    case 12:
        mPos.x = *field_0x284++;
        mPos.y = *field_0x284++;
        mPos.z = *field_0x284++;
        fn_80360914();
        return;
    case 13:
        field_0x294 = 1;
        fn_80360914();
        return;
    case 14:
        field_0x2B0 = 0;
        fn_80360914();
        return;
    case 15:
        field_0x2B0 = 1;
        fn_80360914();
        return;
    default:
        return;
    }
    field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
    field_0x248 = field_0x244;
    if (field_0x270 == 0) {
        updateFacing();
        field_0x244 = lbl_80474020[field_0x274];
        if (field_0x2B0 == 1) {
            pickStepTable();
        }
    }
}

// FIXME(shotgun): 96.6 %, missing/extra
// update() step 0: flies along the current command (fn_80360914).
void KaeporaGaeboraEnding_804ADAC8::fn_8036003C() {
    s32 facing = field_0x274;
    field_0x248 = field_0x244;
    if (field_0x270 == 0) {
        if (advanceStep() == true && field_0x2B0 == 1) {
            field_0x244 = lbl_80474020[field_0x274];
            if (fn_801CD664(100) < 5 ||
                (field_0x250 == 0 && field_0x27C != 0 && field_0x27C != 1 && field_0x26C < 100.0f)) {
                field_0x244++;
            }
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
            if (!(field_0x248 & 1) && (field_0x244 & 1)) {
                mStep = 4;
                field_0x234 = 0;
                advanceStep();
                mStep = 0;
                field_0x234 = 6;
            }
        }
    } else {
        if (field_0x250 == 9 && mPos.y >= 2304.0f) {
            mHeight = -1;
            mDepth = 0;
        } else {
            mHeight = 1;
            mDepth = 1;
        }
        if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x27C == 0) {
        mPos += field_0x3c;
        if (fabs(field_0x288 - mPos.x) < field_0x268 && fabs(field_0x28C - mPos.y) < field_0x268) {
            mPos.x = field_0x288;
            mPos.y = field_0x28C;
            mPos.z = field_0x290;
            fn_80360914();
            return;
        }
    } else if (field_0x27C == 1) {
        mPos += field_0x3c;
        field_0x25C += PSVECMag(&field_0x3c);
        if (field_0x25C >= field_0x258) {
            field_0x25C = 0.0f;
            fn_80360914();
            return;
        }
    } else if (field_0x27C == 2 || field_0x27C == 4) {
        s32 step = (s32)(65536.0f * (JMAFastReciprocal(6.283184f * field_0x26C) * field_0x268));
        JGeometry::TVec3f savedPos = mPos;
        if (field_0x27C == 2) {
            field_0x254 += step;
        } else {
            field_0x254 -= step;
        }
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        mPos += field_0x3c;
        field_0x264 += (s16)step;
        if (field_0x264 >= field_0x260) {
            s16 over = field_0x264 - field_0x260;
            if (field_0x27C == 2) {
                over = field_0x260 - field_0x264;
            }
            if (over != 0) {
                mPos = savedPos;
                field_0x254 += over;
                field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
                field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
                mPos += field_0x3c;
            }
            fn_80360914();
            return;
        }
    } else {
        s32 step = (s32)(65536.0f * (JMAFastReciprocal(6.283184f * field_0x26C) * field_0x268));
        if (field_0x27C == 3) {
            field_0x254 += step;
        } else {
            field_0x254 -= step;
        }
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        mPos += field_0x3c;
        field_0x264 += (s16)step;
        if (field_0x264 >= field_0x260) {
            fn_80360914();
            return;
        }
    }
    updateFacing();
    if (field_0x270 == 0 && field_0x2B0 == 1 && field_0x274 != facing) {
        field_0x244 = lbl_80474020[field_0x274];
        pickStepTable();
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FF70() {
    if (field_0x2B0 == 1) {
        if (field_0x270 == 0) {
            mSway.fn_8021A570(true);
        } else if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x280 == 0) {
        fn_80360914();
    }
}

// FIXME(shotgun): 98.4 %, scheduling+regalloc
void KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C() {
    if (advanceStep() == true) {
        mPtmfIndex = 0;
        field_0x27C = 0;
        aimAtTarget();
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        u16 a = field_0x254;
        if (a >= 0x2000 && a < 0x6000) {
            field_0x274 = 3;
        } else if (a >= 0x6000 && a < 0xa000) {
            field_0x274 = 1;
        } else if (a >= 0xa000 && a < 0xe000) {
            field_0x274 = 2;
        } else {
            field_0x274 = 0;
        }
        field_0x244 = lbl_80474020[field_0x274];
        field_0x234 = 0;
        mStep = 0;
        advanceStep();
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8() {
    if (advanceStep() == true) {
        fn_80360914();
    }
}

// FIXME(shotgun): 97.9 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035FB64() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C < 16.0f) {
        field_0x26C += 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (JMAFastReciprocal(6.283184f * field_0x26C) * 1.75f));
    field_0x254 = prev - step;
    field_0x3c.x = 1.75f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.75f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = 0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C >= 16.0f) {
        fn_80360914();
        field_0x3c.z = 0.0f;
    }
}


// Runs the current waypoint's action and moves to the next one.
inline bool KaeporaGaeboraEnding_804ADAC8::fireWaypoint() {
    const KaeporaWaypoint* wp = field_0x298;
    if (wp->mKind < 0) {
        if (wp->mKind == -1) {
            field_0x29C->fn_802E948C(wp->mX, (s32)wp->mY);
        } else {
            field_0x230 = wp->mKind + 5;
        }
    } else {
        s8 id = wp->mKind;
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            cls_804ADF28* actor = (cls_804ADF28*)getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_EETC && id == actor->field_0x230) {
                actor->field_0x230 = 0;
                break;
            }
        }
    }
    field_0x294 = field_0x298->mNext;
    field_0x298++;
    if (field_0x298->mKind == -1000) {
        field_0x298 = NULL;
    }
    return true;
}

// FIXME(shotgun): 98.1 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C > 3.0f) {
        field_0x26C -= 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (JMAFastReciprocal(6.283184f * field_0x26C) * 1.3f));
    field_0x254 = prev - step;
    field_0x3c.x = 1.3f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.3f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = -0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C < 3.0f) {
        fn_803613D0();
        mAlive = false;
    }
}

// Not in the binary (fn_80360914 and fn_8035FD6C inline it); the pool has its
// 10430.378f ahead of fn_8035F9C8's literals (tools/pool.py), so it sat as a
// plain function below fn_8035F9C8. Turns field_0x254 toward the target
// (field_0x288, field_0x28C).
void KaeporaGaeboraEnding_804ADAC8::aimAtTarget() {
    field_0x254 = (s32)(JMath::atanTable_.atan2_(field_0x28C - mPos.y, field_0x288 - mPos.x) * 10430.378f);
}

// FIXME(shotgun): 98.4 %, scheduling+regalloc
u8 KaeporaGaeboraEnding_804ADAC8::fn_8035F3C0() {
    if (field_0x294 == 0 || field_0x298 == NULL) {
        return false;
    }
    JGeometry::TVec3f pos = mPos;
    pos.z = 0.0f;
    JGeometry::TVec3f target;
    const KaeporaWaypoint* wp = field_0x298;
    if (wp->mKind == -1) {
        if (wp->mRadius == -1.0f || wp->mRadius == -2.0f) {
            target.x = wp->mZ;
            target.y = 0.0f;
            target.z = 0.0f;
        } else if (wp->mRadius == -3.0f || wp->mRadius == -4.0f) {
            target.y = wp->mZ;
            target.x = 0.0f;
            target.z = 0.0f;
        } else {
            field_0x298++;
            if (field_0x298->mKind == -1000) {
                field_0x298 = NULL;
            }
            return true;
        }
    } else {
        target.x = wp->mX;
        target.y = wp->mY;
        target.z = wp->mZ;
    }
    f32 radius = field_0x298->mRadius;
    if (radius > 0.0f) {
        if (PSVECDistance(&pos, &target) < field_0x298->mRadius) {
            return fireWaypoint();
        }
    } else if (radius == -1.0f) {
        if (pos.x < target.x) {
            return fireWaypoint();
        }
    } else if (radius == -2.0f) {
        if (pos.x > target.x) {
            return fireWaypoint();
        }
    } else if (radius == -3.0f) {
        if (pos.y < target.y) {
            return fireWaypoint();
        }
    } else {
        if (pos.y > target.y) {
            return fireWaypoint();
        }
    }
    return false;
}

KaeporaGaeboraEnding_804ADAC8::~KaeporaGaeboraEnding_804ADAC8() {}
