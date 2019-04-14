#include "stdafx.h"
#include "Mudokon.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "AbilityRing.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "DDCheat.hpp"
#include "WorkWheel.hpp"
#include "BirdPortal.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Spark.hpp"
#include "Midi.hpp"
#include "ScreenManager.hpp"

ALIVE_VAR(1, 0x5C3012, short, word_5C3012, 0);

const TintEntry kMudTints_55C744[18] =
{
    { 1, 87u, 103u, 67u },
    { 2, 87u, 103u, 67u },
    { 3, 87u, 103u, 67u },
    { 4, 87u, 103u, 67u },
    { 5, 87u, 103u, 67u },
    { 6, 87u, 103u, 67u },
    { 7, 87u, 103u, 67u },
    { 8, 87u, 103u, 67u },
    { 9, 87u, 103u, 67u },
    { 10, 87u, 103u, 67u },
    { 11, 87u, 103u, 67u },
    { 12, 87u, 103u, 67u },
    { 13, 87u, 103u, 67u },
    { 14, 87u, 103u, 67u },
    { -1, 87u, 103u, 67u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

using TMudAIStateFunction = decltype(&Mudokon::AI_Give_rings_0_470C10);

const TMudAIStateFunction sMudokon_AI_Table_55CDF0[10] =
{
    &Mudokon::AI_Give_rings_0_470C10,
    &Mudokon::AI_Chisel_1_47C5F0,
    &Mudokon::AI_Scrub_2_47D270,
    &Mudokon::AI_State_3_47E0D0,
    &Mudokon::AI_Wired_4_477B40,
    &Mudokon::AI_ShrivelDeath_5_4714A0,
    &Mudokon::AI_HelloAlerted_6_47A560,
    &Mudokon::AI_FallAndSmackDeath_7_471600,
    &Mudokon::AI_AngryWorker_8_47E910,
    &Mudokon::AI_Sick_9_47A910
};

#define MUD_MOTION_STATES_ENUM(ENTRY) \
    ENTRY(StandIdle_0_4724E0) \
    ENTRY(Walking_1_4728B0) \
    ENTRY(TurnAroundStanding_2_472BF0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(Speak_Generic_472FA0) \
    ENTRY(StandToWalk_7_472AB0) \
    ENTRY(WalkingToStand_8_472B30) \
    ENTRY(jWalkingToStand_8_472BD0) \
    ENTRY(PullLever_10_473020) \
    ENTRY(Chisel_11_4732D0) \
    ENTRY(StartChisel_12_473500) \
    ENTRY(StopChisel_13_473530) \
    ENTRY(CrouchScrub_14_473560) \
    ENTRY(CrouchIdle_15_474040) \
    ENTRY(CrouchTurn_16_4740E0) \
    ENTRY(StandToCrouch_17_474120) \
    ENTRY(CrouchToStand_18_474150) \
    ENTRY(StandingToRun1_19_473600) \
    ENTRY(StandingToRun2_20_4736D0) \
    ENTRY(Running_21_473720) \
    ENTRY(RunToWalk1_22_4738E0) \
    ENTRY(RunToWalk2_23_4739B0) \
    ENTRY(RunSlideStop_24_473A00) \
    ENTRY(RunSlideTurn_25_473AA0) \
    ENTRY(RunTurn_26_473BB0) \
    ENTRY(Sneaking_27_473C40) \
    ENTRY(WalkToSneak1_28_473D60) \
    ENTRY(SneakingToWalk_29_473E20) \
    ENTRY(WalkToSneak2_30_473EE0) \
    ENTRY(SneakingToWalk2_31_473F30) \
    ENTRY(StandToSneaking_32_473F80) \
    ENTRY(SneakingToStand1_33_473FF0) \
    ENTRY(SneakingToStand2_34_474020) \
    ENTRY(JumpStart_35_474460) \
    ENTRY(JumpMid_36_474570) \
    ENTRY(WalkToRun_37_4749A0) \
    ENTRY(Slap_38_474AA0) \
    ENTRY(StartHoistJumpUp_39_4748E0) \
    ENTRY(HoistFallToFloor_40_474780) \
    ENTRY(HitFloorStanding1_41_474960) \
    ENTRY(HitFloorStanding2_42_4743F0) \
    ENTRY(StandToDunno_43_472790) \
    ENTRY(DunnoToStand_44_4727B0) \
    ENTRY(KnockForward_45_474180) \
    ENTRY(StandToKnockBack_46_4742A0) \
    ENTRY(KnockBackToStand_47_474380) \
    ENTRY(FallLedgeBegin_48_4743C0) \
    ENTRY(Fall_49_472C60) \
    ENTRY(Chanting_50_473040) \
    ENTRY(ToChant_51_4730D0) \
    ENTRY(ToDuck_52_474A20) \
    ENTRY(Duck_53_474A40) \
    ENTRY(DuckToCrouch_54_474A80) \
    ENTRY(DuckKnockBack_55_474250) \
    ENTRY(SlapOwnHead_56_4727D0) \
    ENTRY(TurnWheelBegin_57_474C00) \
    ENTRY(TurnWheelLoop_58_474CC0) \
    ENTRY(TurnWheelEnd_59_474D30)

const char* const sMudMotionStateNames[60] =
{
    MUD_MOTION_STATES_ENUM(MAKE_STRINGS)
};

struct MudEmotionTableEntry
{
    __int16 field_0_sound; 
    Mud_Motion field_2_next_motion;
    Mud_Emotion field_4_emo_tbl;
    __int16 field_6_sub_state;
};

struct MudEmotionTable
{
    MudEmotionTableEntry field_0_data[8][17];
};

const MudEmotionTable kMudEmoTable_55C790 =
{
    {
        {
            { 3,  Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 23, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eNormal_0,  12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  12 }
        },
        {
            { 18, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { 21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            { 18, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { 21, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eAngry_1,   12 },
            { 5,  Mud_Motion::Slap_38_474AA0,             Mud_Emotion::eUnknown_2, 13 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eAngry_1,   0 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eAngry_1,   12 }
        },
        {
            { 20, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSad_3,     0 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eSad_3,     12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 13 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  15 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 }
        },
        {
            { 20, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 13 },
            { 22, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eNormal_0,  0 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eUnknown_4, 12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eUnknown_4, 0 }
        },
        {
            { 19, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::StartHoistJumpUp_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 13, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eWired_6,   12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { 10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            { 19, Mud_Motion::Speak_Generic_4_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::StartHoistJumpUp_39_4748E0, Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 13, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 5,  Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 12, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eNormal_0,  13 },
            { -1, Mud_Motion::StandToDunno_43_472790,     Mud_Emotion::eWired_6,   12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eWired_6,   0 },
            { 10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 10, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eWired_6,   12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 22, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSad_3,     12 },
            { 10, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eWired_6,   12 }
        },
        {
            { 24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { 24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { 24, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { 24, Mud_Motion::Speak_Generic_3_472FA0,     Mud_Emotion::eSick_7, 12 },
            { 24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { 28, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 13 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { -1, Mud_Motion::StandIdle_0_4724E0,         Mud_Emotion::eSick_7, 0 },
            { 24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 },
            { 24, Mud_Motion::Speak_Generic_6_472FA0,     Mud_Emotion::eSick_7, 12 }
        }
    }
};



using TMudStateFunction = decltype(&Mudokon::StandIdle_0_4724E0);

const TMudStateFunction sMudokon_motion_states_55CE18[60] =
{
    &Mudokon::StandIdle_0_4724E0,
    &Mudokon::Walking_1_4728B0,
    &Mudokon::TurnAroundStanding_2_472BF0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::Speak_Generic_472FA0,
    &Mudokon::StandToWalk_7_472AB0,
    &Mudokon::WalkingToStand_8_472B30,
    &Mudokon::jWalkingToStand_8_472BD0,
    &Mudokon::PullLever_10_473020,
    &Mudokon::Chisel_11_4732D0,
    &Mudokon::StartChisel_12_473500,
    &Mudokon::StopChisel_13_473530,
    &Mudokon::CrouchScrub_14_473560,
    &Mudokon::CrouchIdle_15_474040,
    &Mudokon::CrouchTurn_16_4740E0,
    &Mudokon::StandToCrouch_17_474120,
    &Mudokon::CrouchToStand_18_474150,
    &Mudokon::StandingToRun1_19_473600,
    &Mudokon::StandingToRun2_20_4736D0,
    &Mudokon::Running_21_473720,
    &Mudokon::RunToWalk1_22_4738E0,
    &Mudokon::RunToWalk2_23_4739B0,
    &Mudokon::RunSlideStop_24_473A00,
    &Mudokon::RunSlideTurn_25_473AA0,
    &Mudokon::RunTurn_26_473BB0,
    &Mudokon::Sneaking_27_473C40,
    &Mudokon::WalkToSneak1_28_473D60,
    &Mudokon::SneakingToWalk_29_473E20,
    &Mudokon::WalkToSneak2_30_473EE0,
    &Mudokon::SneakingToWalk2_31_473F30,
    &Mudokon::StandToSneaking_32_473F80,
    &Mudokon::SneakingToStand1_33_473FF0,
    &Mudokon::SneakingToStand2_34_474020,
    &Mudokon::JumpStart_35_474460,
    &Mudokon::JumpMid_36_474570,
    &Mudokon::WalkToRun_37_4749A0,
    &Mudokon::Slap_38_474AA0,
    &Mudokon::StartHoistJumpUp_39_4748E0,
    &Mudokon::HoistFallToFloor_40_474780,
    &Mudokon::HitFloorStanding1_41_474960,
    &Mudokon::HitFloorStanding2_42_4743F0,
    &Mudokon::StandToDunno_43_472790,
    &Mudokon::DunnoToStand_44_4727B0,
    &Mudokon::KnockForward_45_474180,
    &Mudokon::StandToKnockBack_46_4742A0,
    &Mudokon::KnockBackToStand_47_474380,
    &Mudokon::FallLedgeBegin_48_4743C0,
    &Mudokon::Fall_49_472C60,
    &Mudokon::Chanting_50_473040,
    &Mudokon::ToChant_51_4730D0,
    &Mudokon::ToDuck_52_474A20,
    &Mudokon::Duck_53_474A40,
    &Mudokon::DuckToCrouch_54_474A80,
    &Mudokon::DuckKnockBack_55_474250,
    &Mudokon::SlapOwnHead_56_4727D0,
    &Mudokon::TurnWheelBegin_57_474C00,
    &Mudokon::TurnWheelLoop_58_474CC0,
    &Mudokon::TurnWheelEnd_59_474D30
};


// This is used rather than the un-typesafe word_55CF08 array
static Mud_Emotion TLV_Emo_To_Internal_Emo(Mud_TLV_Emotion emo)
{
    switch (emo)
    {
    case Mud_TLV_Emotion::eNormal_0:
        return Mud_Emotion::eNormal_0;

    case Mud_TLV_Emotion::eAngry_1:
        return Mud_Emotion::eAngry_1;

    case Mud_TLV_Emotion::eSad_2:
        return Mud_Emotion::eSad_3;

    case Mud_TLV_Emotion::eWired_3:
        return Mud_Emotion::eWired_6;

    case Mud_TLV_Emotion::eSick_4:
        return Mud_Emotion::eSick_7;

    default:
        LOG_ERROR("Don't know about emo type " << static_cast<int>(emo));
        ALIVE_FATAL("Unknown emo");
    }
}

Mudokon* Mudokon::ctor_474F30(Path_Mudokon* pTlv, int tlvInfo)
{
    ctor_408240(18);

    field_154 = 0;
    field_140 = -1;
    field_156 = -1;

    SetVTable(this, 0x5462E4);

    field_11C_bird_portal_id = -1;
    field_12C = -1;
    field_158_wheel_id = -1;
    field_4_typeId = Types::eMudokon2_81; // TODO: Set to 110 later, what is 81 ??
    field_C_objectId = tlvInfo;
    field_194_timer = 0;
    field_18E_ai_state = Mud_AI_State::AI_Give_rings_0_470C10;
    field_190_sub_state = 0;
    field_108_next_motion = -1;
    field_192 = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);
    Animation_Init_424E10(58888, 135, 80, field_10_resources_array.ItemAt(0), 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows |= 2u;

    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C = 0;
    SetTint_425600(kMudTints_55C744, field_C2_lvl_number);

    field_17E_delayed_speak = MudAction::eUnknown_17;

    field_16A_flags.Clear(Flags::eBit2_save_state);
    field_16A_flags.Clear(Flags::eBit3);
    field_16A_flags.Set(Flags::eBit4_blind, pTlv->field_22_bBlind & 1);
    field_16A_flags.Clear(Flags::eBit5);
    field_16A_flags.Clear(Flags::eBit6);
    field_16A_flags.Clear(Flags::eBit7);
    field_16A_flags.Clear(Flags::eBit8);
    field_16A_flags.Clear(Flags::eBit9);
    // 10 and 11 ??
    field_16A_flags.Set(Flags::eBit12);
    field_16A_flags.Clear(Flags::eBit13);
    field_16A_flags.Clear(Flags::eBit14);

    field_180_emo_tbl = TLV_Emo_To_Internal_Emo(pTlv->field_20_emotion);
    field_188_pTblEntry = nullptr;
    field_182 = -1;
    field_120_angry_trigger = pTlv->field_24_angry_trigger;
    field_16C &= ~6u;
    field_198 = 0;

    switch (pTlv->field_12_state)
    {
    case Mud_State::eChisle_0:
        field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
        field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudchslResID, TRUE, FALSE));
        break;

    case Mud_State::eScrub_1:
        field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudscrubResID, TRUE, FALSE));
        break;

    case Mud_State::eAngryWorker_2:
        field_18E_ai_state = Mud_AI_State::AI_AngryWorker_8_47E910;
        field_180_emo_tbl = Mud_Emotion::eAngry_1;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudscrubResID, TRUE, FALSE));
        break;

    case Mud_State::eDamageRingGiver_3:
    case Mud_State::eHealthRingGiver_4:
        if (pTlv->field_12_state == Mud_State::eDamageRingGiver_3)
        {
            field_168_ring_type = RingTypes::eExplosive_Emit_Effect_2;
        }
        else
        {
            field_168_ring_type = RingTypes::eHealing_Emit_Effect_11;
        }
        field_164_ring_timeout = pTlv->field_2A_ring_timeout;
        field_16A_flags.Set(Flags::eBit16_instant_power_up, pTlv->field_2C_bInstant_power_up & 1);
        field_16C &= ~1;
        field_18E_ai_state = Mud_AI_State::AI_Give_rings_0_470C10;
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE));
        break;

    default:
        break;
    }

    if (field_180_emo_tbl == Mud_Emotion::eWired_6)
    {
        field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
    }

    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudoduckResID, TRUE, FALSE));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudbtlnkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMudidleResID, TRUE, FALSE));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudangryResID, TRUE, FALSE));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudsadResID, TRUE, FALSE));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudwiredResID, TRUE, FALSE));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudblindResID, TRUE, FALSE));
    field_10_resources_array.SetAt(17, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kMudsickResID, TRUE, FALSE));

    if (field_16A_flags.Get(Flags::eBit4_blind))
    {
        SetPal_4772D0(Mud_Emotion::eNormal_0);
    }

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_14_direction == Mud_Direction::eLeft_0);

    field_4_typeId = Types::eMudokon_110;

    field_13C_voice_pitch = pTlv->field_16_voice_pitch;
    field_17A_rescue_id = pTlv->field_18_rescue_id;

    field_16A_flags.Set(Flags::eBit2_save_state, pTlv->field_1E_save_state & 1);
    field_16A_flags.Set(Flags::eBit10_stop_trigger, pTlv->field_26_stop_trigger & 1);
    field_16A_flags.Set(Flags::eBit11_get_depressed, pTlv->field_28_bGets_depressed & 1);
    field_16A_flags.Set(Flags::eBit15_ring_timeout, pTlv->field_2A_ring_timeout & 1);

    field_17C = 0;

    vStackOnObjectsOfType_425840(Types::eMudokon_110);

    if (field_180_emo_tbl == Mud_Emotion::eSick_7)
    {
        field_18E_ai_state = Mud_AI_State::AI_Sick_9_47A910;
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
    else
    {
        field_106_current_motion = Mud_Motion::StandIdle_0_4724E0;
    }

    if (pTlv->field_10_scale == TLV_Scale::Half_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 9;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == TLV_Scale::Full_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 28;
        field_D6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const short bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16);

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    if (bCollision)
    {
        field_BC_ypos = hitY;
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    field_20_animation.field_1C_fn_ptr_array = kAbe_Anim_Frame_Fns_55EF98;
    field_F8 = field_BC_ypos;

    if (field_18E_ai_state == Mud_AI_State::AI_Scrub_2_47D270 || field_18E_ai_state == Mud_AI_State::AI_Chisel_1_47C5F0)
    {
        MapFollowMe_408D10(TRUE);
    }

    field_16A_flags.Set(Flags::eBit1);
    field_190_sub_state = 0;
    field_118 = field_C_objectId;

    field_128 = 0;
    field_160_delayed_speak = -1;
    field_162 = field_DA_xOffset;
    
    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    vUpdate_4757A0();

    return this;
}

void Mudokon::VUpdate()
{
    vUpdate_4757A0();
}

void Mudokon::vUpdate_4757A0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);

            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                vOnCollisionWith_424EE0(
                    { static_cast<short>(bRect.x + 5), bRect.y },
                    { static_cast<short>(bRect.w + 5), bRect.h }, 
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }

        field_104_collision_line_type = 0;

        if (field_11C_bird_portal_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_11C_bird_portal_id)
                {
                    field_11C_bird_portal_id = pObj->field_8_object_id;
                    word_5C3012++;
                    field_16C |= 4u;
                    if (field_18E_ai_state == Mud_AI_State::AI_HelloAlerted_6_47A560 && field_190_sub_state == 3)
                    {
                        static_cast<BirdPortal*>(pObj)->Vsub_499430(1);
                        field_20_animation.field_C_render_layer = field_CC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                    }
                    break;
                }
            }
        }

        if (field_158_wheel_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_158_wheel_id)
                {
                    field_158_wheel_id = pObj->field_8_object_id;
                    static_cast<WorkWheel*>(pObj)->VStartTurning();
                    break;
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDistFromPlayer = FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos);
    if (xDistFromPlayer > FP_FromInteger(750))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    const FP yDistanceFromPlayer = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);
    if (yDistanceFromPlayer > FP_FromInteger(520))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        return;
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    }

    //DDCheat::DebugStr_4F5560("\nMotion = %s BrainState = %s\n", sMudMotionStateNames[field_106_current_motion], sMudAiStateNames[static_cast<int>(field_18E_ai_state)]);
    //LOG_INFO(sMudMotionStateNames[field_106_current_motion] << " " << static_cast<int>(field_18E_ai_state));
    //LOG_INFO(sMudMotionStateNames[field_106_current_motion] << " " << sMudAiStateNames[static_cast<int>(field_18E_ai_state)]);

    const __int16 oldMotion = field_106_current_motion;
    field_190_sub_state = (this->*sMudokon_AI_Table_55CDF0[field_18E_ai_state])();
   
    const FP oldXPos = field_B8_xpos;
    const FP oldYPos = field_BC_ypos;

    (this->*sMudokon_motion_states_55CE18[field_106_current_motion])();

    if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
        VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
    }

    if (oldMotion != field_106_current_motion || field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
    {
        field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
        vUpdateAnimRes_474D80();
    }
    else if (field_192)
    {
        field_106_current_motion = field_F4;
        vUpdateAnimRes_474D80();
        field_20_animation.SetFrame_409D50(field_F6_anim_frame);
        field_192 = 0;
    }
}


void Mudokon::SetPal_4772D0(Mud_Emotion emotion)
{
    switch (emotion)
    {
    case Mud_Emotion::eNormal_0:
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            SetRGB(63, 63, 63);
            field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(16), 0);
        }
        else
        {
            SetRGB(87, 103, 67);
            FrameInfoHeader* pFrameInfoHeader = field_20_animation.Get_FrameHeader_40B730(-1);
            FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
            field_20_animation.Load_Pal_40A530(field_20_animation.field_20_ppBlock, pHeader->field_0_clut_offset);
        }
        break;

    case Mud_Emotion::eAngry_1:
    case Mud_Emotion::eUnknown_2:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(13), 0);
        break;

    case Mud_Emotion::eSad_3:
    case Mud_Emotion::eUnknown_4:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(14), 0);
        break;

    case Mud_Emotion::eHappy_5:
    case Mud_Emotion::eWired_6:
        SetRGB(74, 74, 74);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(15), 0);
        break;

    case Mud_Emotion::eSick_7:
        SetRGB(63, 63, 63);
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(17), 0);
        break;
    }
}

void Mudokon::vOnTrapDoorOpen_472350()
{
    NOT_IMPLEMENTED();

    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        if (!field_114_flags.Get(Flags_114::e114_Bit1))
        {
            VSetMotion_4081C0(Mud_Motion::FallLedgeBegin_48_4743C0);
        }

        pPlatform->VRemove(this);
        field_110_id = -1;
    }
}

const int kMudFrameTableOffsets_55CD00[60] =
{
    58888,    58808,    59064,    59028,    58956,
    58992,    58920,    58768,    58788,    58748,
    11396,    5276,     5308,     5320,     9388,
    270092,   270428,   269876,   270060,   270120,
    270268,   270748,   270668,   270820,   270860,
    270328,   270408,   270508,   270688,   270596,
    270840,   270728,   270288,   270160,   270308,
    270468,   269804,   270252,   271152,   269976,
    270024,   269928,   269928,   270180,   270616,
    17240,    49740,    49800,    32040,    32012,
    9992,     10040,    5236,     5256,     5280,
    5328,     9640,     11856,    11816,    11888
};


void Mudokon::vUpdateAnimRes_474D80()
{
    BYTE** ppRes = AnimBlockForMotion_474DC0(field_106_current_motion);
    if (!ppRes)
    {
        LOG_ERROR("No res for " << field_106_current_motion);
    }
    field_20_animation.Set_Animation_Data_409C80(kMudFrameTableOffsets_55CD00[field_106_current_motion], ppRes);
}

__int16 Mudokon::AI_Give_rings_0_470C10()
{
    NOT_IMPLEMENTED();
    return 0;
}

enum BrainStates
{
    eState_StandToCrouch_0 = 0,
    eState_ChisleTheFloor_1 = 1,
    eState_CrouchToStand_2 = 2,
    eState_3 = 3,
    eState_SadStandUp_4 = 4,
    eState_Duck_5 = 5,
    eState_OutOfDuck_6 = 6,
    eState_DuckToChisle_7 = 7,
    eState_DuckKnockBack_8 = 8,
    eState_9 = 9,
    eState_10 = 10,
};

const __int16 kDelayTable_55CF7C[6] = { 0, 6, 12, 18, 24, 30, };
ALIVE_VAR(1, 0x5C3014, short, sDelayIdx_5C3014, 0);
ALIVE_VAR(1, 0x5C3010, short, word_5C3010, 0);

ALIVE_VAR(1, 0x5C3018, short, word_5C3018, 0);

signed __int16 Mudokon::AI_Chisel_1_47C5F0()
{
    if (CheckForPortal_4775E0())
    {
        if (!field_16A_flags.Get(Flags::eBit3))
        {
            field_16A_flags.Set(Flags::eBit3);
            word_5C3010++;
        }
        field_18E_ai_state = Mud_AI_State::AI_HelloAlerted_6_47A560;
        return BrainStates::eState_StandToCrouch_0;
    }

    if (field_16C & 2)
    {
        if (field_190_sub_state != BrainStates::eState_3)
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
        }
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
    if (lastSpeak == GameSpeakEvents::eLookOut_6 || Event_Get_422C00(kEventShooting))
    {
        if (field_190_sub_state != BrainStates::eState_3 && field_190_sub_state != BrainStates::eState_Duck_5 && field_190_sub_state != BrainStates::eState_DuckKnockBack_8 && field_190_sub_state != BrainStates::eState_OutOfDuck_6)
        {
            field_190_sub_state = BrainStates::eState_Duck_5;
        }
    }

    BaseAnimatedWithPhysicsGameObject* pAbuseEvent = Event_Is_Event_In_Range_422C30(kEventMudokonAbuse, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pDeadMudEvent = Event_Is_Event_In_Range_422C30(kEventMudokonDied, field_B8_xpos, field_BC_ypos, field_D6_scale);
    BaseAnimatedWithPhysicsGameObject* pLoudNoiseEvent = Event_Is_Event_In_Range_422C30(kEventLoudNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);

    const bool reactToAbused = (pAbuseEvent && pAbuseEvent != this && field_190_sub_state != BrainStates::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToDeadMud = (pDeadMudEvent && pDeadMudEvent != this && field_190_sub_state != BrainStates::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    const bool reactToLoudNoise = (pLoudNoiseEvent && pLoudNoiseEvent->field_4_typeId == Types::eGlukkon_67 && field_190_sub_state != BrainStates::eState_3 && 
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0));

    if (reactToAbused)
    {
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_9);
    }

    if (reactToDeadMud)
    {
        field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eMudDied_14);
    }

    if (reactToLoudNoise)
    {
        if (field_16A_flags.Get(Flags::eBit13))
        {
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_16);
        }
        else
        {
            field_16A_flags.Set(Flags::eBit13);
            field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, MudAction::eUnknown_15);
        }
    }

    if (reactToAbused || reactToDeadMud || reactToLoudNoise)
    {
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }

        if (field_180_emo_tbl == Mud_Emotion::eUnknown_4 || field_180_emo_tbl == Mud_Emotion::eSad_3)
        {
            if (!field_16A_flags.Get(Flags::eBit3))
            {
                word_5C3010++;
                field_16A_flags.Set(Flags::eBit3);
            }

            if (field_180_emo_tbl == Mud_Emotion::eSad_3)
            {
                field_16A_flags.Set(Flags::eBit6);
            }

            field_190_sub_state = BrainStates::eState_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;

            if (!field_16A_flags.Get(Flags::eBit3))
            {
                word_5C3010++;
                field_16A_flags.Set(Flags::eBit3);
            }

            field_190_sub_state = BrainStates::eState_3;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128)
    {
        if (SwitchStates_Get_466020(field_120_angry_trigger))
        {
            field_128 = sGnFrame_5C1B84 + 20;
        }
    }

    if (!field_16A_flags.Get(Flags::eBit8))
    {
        if (field_128 > 0)
        {
            if (field_128 <= static_cast<int>(sGnFrame_5C1B84))
            {
                field_16A_flags.Set(Flags::eBit8);
                field_180_emo_tbl = Mud_Emotion::eAngry_1;
                field_190_sub_state = BrainStates::eState_3;
                field_194_timer = sGnFrame_5C1B84;
            }
        }
    }

    switch (field_190_sub_state)
    {
    case BrainStates::eState_StandToCrouch_0:
        if (field_106_current_motion != Mud_Motion::StandToCrouch_17_474120)
        {
            field_106_current_motion = Mud_Motion::Chisel_11_4732D0;
            field_108_next_motion = -1;
        }
        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        return BrainStates::eState_ChisleTheFloor_1;

    case BrainStates::eState_ChisleTheFloor_1:
    {
        bool ignoreHellOrAllYa = false;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
            {
                ignoreHellOrAllYa = true;
            }
        }
        else if (lastSpeak != GameSpeakEvents::eAllYa_23)
        {
            ignoreHellOrAllYa = true;
        }

        if (!ignoreHellOrAllYa && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;

            if (!field_16A_flags.Get(Flags::eBit3))
            {
                word_5C3010++;
                field_16A_flags.Set(Flags::eBit3);
            }
            return BrainStates::eState_3;
        }

        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_194_timer && field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        }

        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && (Math_NextRandom() < 120))
        {
            if (!(field_16A_flags.Get(Flags::eBit3)))
            {
                word_5C3010++;
                field_16A_flags.Set(Flags::eBit3);
            }

            field_16A_flags.Set(Flags::eBit14);
            field_16A_flags.Set(Flags::eBit6);

            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            return BrainStates::eState_3;
        }
        else
        {
            field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
            return BrainStates::eState_CrouchToStand_2;
        }
    }
    break;

    case BrainStates::eState_CrouchToStand_2:
    {
        bool ignoreHellOrAllYa = false;
        if (lastSpeak == GameSpeakEvents::eHello_9)
        {
            if (word_5C3018 || !CanRespond_4770B0())
            {
                ignoreHellOrAllYa = true;
            }
        }
        else if (lastSpeak != GameSpeakEvents::eAllYa_23)
        {
            ignoreHellOrAllYa = true;
        }

        if (!ignoreHellOrAllYa && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            if (!field_16A_flags.Get(Flags::eBit3))
            {
                word_5C3010++;
                field_16A_flags.Set(Flags::eBit3);
            }

            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 10;
            word_5C3018 = 1;
            field_16C |= 2u;
            return BrainStates::eState_3;
        }

        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }

        field_194_timer = (Math_NextRandom() % 64) + (sGnFrame_5C1B84 + 35);
        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
    }
    return BrainStates::eState_ChisleTheFloor_1;

    case BrainStates::eState_3:
    {
        bool skip = false;
        bool bForce = false;
        if (lastSpeak != GameSpeakEvents::eHello_9)
        {
            if (lastSpeak == GameSpeakEvents::eAllYa_23)
            {
                bForce = true;
            }
            else
            {
                skip = true;
            }
        }

        if (!skip && (bForce || !word_5C3018 && CanRespond_4770B0()))
        {
            if (sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                if (!(field_16A_flags.Get(Flags::eBit3)))
                {
                    word_5C3010++;
                    field_16A_flags.Set(Flags::eBit3);
                }
                word_5C3018 = 1;
                field_16C |= 2u;

                field_16A_flags.Clear(Flags::eBit14);
                field_16A_flags.Clear(Flags::eBit6);
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer || field_106_current_motion)
        {
            if (field_108_next_motion != -1)
            {
                return field_190_sub_state;
            }

            if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0 || field_106_current_motion == Mud_Motion::CrouchIdle_15_474040)
            {
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            }

            if (field_106_current_motion != Mud_Motion::Duck_53_474A40)
            {
                return field_190_sub_state;
            }
            else
            {
                field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                return field_190_sub_state;
            }
        }

        if (field_180_emo_tbl == Mud_Emotion::eSad_3 && field_16A_flags.Get(Flags::eBit14))
        {
            field_16A_flags.Clear(Flags::eBit12);
            field_16A_flags.Clear(Flags::eBit14);
            field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
            Sound_475EC0(28);
            return BrainStates::eState_SadStandUp_4;
        }
        else
        {
            word_5C3018 = 0;
            field_16C &= ~2u;
            if (field_180_emo_tbl != Mud_Emotion::eAngry_1 || field_16A_flags.Get(Flags::eBit8)) // TODO: Inverted?? Was using SLOBYTE()
            {
                field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
                return BrainStates::eState_StandToCrouch_0;
            }
            else
            {
                field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
                field_18E_ai_state = Mud_AI_State::AI_Wired_4_477B40;
                field_178_sub_state2 = 7;
                return BrainStates::eState_DuckToChisle_7;
            }
        }
    }
    break;

    case BrainStates::eState_SadStandUp_4:
        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            return field_190_sub_state;
        }
        field_16A_flags.Clear(Flags::eBit6);
        field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
        return BrainStates::eState_StandToCrouch_0;

    case BrainStates::eState_Duck_5:
        if (field_106_current_motion == Mud_Motion::StandIdle_0_4724E0)
        {
            field_108_next_motion = Mud_Motion::StandToCrouch_17_474120;
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return field_190_sub_state;
        }

        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }

        field_108_next_motion = Mud_Motion::Duck_53_474A40;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return BrainStates::eState_OutOfDuck_6;

    case BrainStates::eState_OutOfDuck_6:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        return BrainStates::eState_DuckToChisle_7;

    case BrainStates::eState_DuckToChisle_7:
        if (field_106_current_motion == Mud_Motion::Chisel_11_4732D0)
        {
            return BrainStates::eState_StandToCrouch_0;
        }
        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        return field_190_sub_state;

    case BrainStates::eState_DuckKnockBack_8:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::DuckKnockBack_55_474250;
        field_194_timer = sGnFrame_5C1B84 + 60;
        return BrainStates::eState_OutOfDuck_6;

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_Scrub_2_47D270()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_State_3_47E0D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_Wired_4_477B40()
{
    if (CheckForPortal_4775E0())
    {
        field_18E_ai_state = Mud_AI_State::AI_HelloAlerted_6_47A560;
        field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
        return 0;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonAbuse,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_9;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonDied,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eMudDied_14;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonComfort,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_10;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventMudokonLaugh,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_12;
    }

    if (Event_Is_Event_In_Range_422C30(
        kEventShooting | kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        field_17E_delayed_speak = MudAction::eUnknown_13;
    }

    BaseAnimatedWithPhysicsGameObject* pNoiseEvent = Event_Is_Event_In_Range_422C30(
        kEventLoudNoise,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);

    if (pNoiseEvent)
    {
        if (pNoiseEvent->field_4_typeId == Types::eGlukkon_67)
        {
            if (field_16A_flags.Get(Flags::eBit13))
            {
                field_17E_delayed_speak = MudAction::eUnknown_16;
            }
            else
            {
                field_16A_flags.Set(Flags::eBit13);
                field_17E_delayed_speak = MudAction::eUnknown_15;
            }
        }
    }

    __int16 result = 0;
    __int16 v18 = 0;
    __int16 v126 = 0;
    __int16 v5 = 6;
    FP v65 = {};
    int v67 = 0;
    Mudokon* v35 = nullptr;
    int v80 = 0;
    FP v42 = {};

    if (field_180_emo_tbl != Mud_Emotion::eWired_6)
    {
        if (LaughingGasInCurrentScreen_4774A0())
        {
            field_180_emo_tbl = Mud_Emotion::eWired_6;
        }
    }

    if (field_180_emo_tbl != Mud_Emotion::eAngry_1 && !field_128)
    {
        if (SwitchStates_Get_466020(field_120_angry_trigger))
        {
            field_128 = sGnFrame_5C1B84 + 15;
        }
    }

    if (field_16A_flags.Get(Flags::eBit8))
    {
        goto LABEL_31;
    }
    
    int v7 = field_128;
    if (!v7)
    {
        goto LABEL_31;
    }

    int gnFrame = sGnFrame_5C1B84;
    if (v7 <= (signed int)sGnFrame_5C1B84)
    {
        field_16A_flags.Set(Flags::eBit8);
        if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - FP_FromInteger(5)),
            TlvTypes::Water_77))
        {
            field_180_emo_tbl = Mud_Emotion::eAngry_1;
            field_188_pTblEntry = ResponseTo_471730(Mud_Emotion::eAngry_1, MudAction::eSlapOrWater_7);
            field_190_sub_state = 14;
            field_178_sub_state2 = 7;
            gnFrame = sGnFrame_5C1B84;
            v5 = 6;
            goto LABEL_32;
        }
        v5 = 6;
    LABEL_31:
        gnFrame = sGnFrame_5C1B84;
    }
LABEL_32:
    switch (field_190_sub_state)
    {
    case 0u:
        field_108_next_motion = -1;
        ToStand_4724A0();
        field_178_sub_state2 = -1;
        return 1;

    case 1u:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }

        if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            goto LABEL_438;
        }

        switch (field_180_emo_tbl)
        {
        case Mud_Emotion::eNormal_0:
            Sound_475EC0(3);
            break;

        case Mud_Emotion::eAngry_1:
        case Mud_Emotion::eUnknown_2:
            if (field_16A_flags.Get(Flags::eBit6))
            {
                Sound_475EC0(5);
            }
            else
            {
                Sound_475EC0(18);
            }
            break;

        case Mud_Emotion::eSad_3:
        case Mud_Emotion::eSick_7:
            if (field_16A_flags.Get(Flags::eBit6))
            {
                goto LABEL_42;
            }
            Sound_475EC0(20);
            break;

        case Mud_Emotion::eUnknown_4:
        LABEL_42:
            Sound_475EC0(22);
            break;

        case Mud_Emotion::eHappy_5:
        case Mud_Emotion::eWired_6:
            Sound_475EC0(10);
            break;

        default:
            break;
        }

        field_108_next_motion = Mud_Motion::Speak_Generic_4_472FA0;
        if (!field_16A_flags.Get(Flags::eBit6) || field_180_emo_tbl == Mud_Emotion::eWired_6)
        {
            goto LABEL_364;
        }

        field_16A_flags.Clear(Flags::eBit6);
        field_16A_flags.Clear(Flags::eBit12);

        if (field_10_resources_array.ItemAt(2))
        {
            field_184_motion = Mud_Motion::Chisel_11_4732D0;
        }
        else
        {
            field_184_motion = Mud_Motion::CrouchIdle_15_474040;
        }
        field_178_sub_state2 = 22;
        field_16A_flags.Clear(Flags::eBit5);
        return 10;

    case 2u:
        if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            goto LABEL_438;
        }

        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            if (field_106_current_motion == Mud_Motion::Walking_1_4728B0)
            {
                if (field_16A_flags.Get(Flags::eBit4_blind))
                {
                    return 5;
                }

                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                {
                    return 5;
                }
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
                result = 4;
            }
            else
            {
                if (field_106_current_motion != Mud_Motion::StandToKnockBack_46_4742A0)
                {
                    return field_190_sub_state;
                }
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
                result = 4;
            }
        }
        else
        {
            result = field_190_sub_state;
            field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        }
        return result;

    case 3u:
        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            if (field_106_current_motion != Mud_Motion::Walking_1_4728B0)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            abort(); // TODO FIX ME
            /*
            v91 = -(field_16A_flags.Get(Flags::eBit5) != 0);
            LOBYTE(v91) = v91 & 0xFD;
            result = v91 + 7;*/
        }
        else
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                goto LABEL_438;
            }
            result = field_190_sub_state;
            field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        }
        return result;

    case 4u:
        if (field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            field_16A_flags.Clear(Flags::eBit5);
            result = 7;
        }
        else
        {
            if (field_106_current_motion == Mud_Motion::Running_21_473720)
            {
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            }
            if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
            {
                goto LABEL_64;
            }

            if (sActiveHero_5C1B68->field_B8_xpos >= field_B8_xpos)
            {
                v42 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                v42 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            const int v44 = Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v42);

            if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
            {
                field_17C = 0;
                field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
                result = field_190_sub_state;
            }
            else
            {
                field_16A_flags.Clear(Flags::eBit7);
                if (++field_17C <= 540 || field_180_emo_tbl == Mud_Emotion::eWired_6)
                {
                    bool bContinue = word_5C3010 <= 1;
                    if (!bContinue)
                    {
                        auto pObj = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                        if (pObj)
                        {
                            if (vIsFacingMe_4254A0(pObj) || pObj->field_106_current_motion == Mud_Motion::TurnAroundStanding_2_472BF0)
                            {
                                bContinue = true;
                            }
                        }
                    }

                    if (bContinue)
                    {
                        if (vIsFacingMe_4254A0(sActiveHero_5C1B68) || word_5C3010 > 1 && FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
                        {
                            const FP v48 = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2);
                            if (vIsObjNearby_4253B0(v48, sActiveHero_5C1B68) && !(field_16A_flags.Get(Flags::eBit4_blind)) || v44 || FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                            {
                                const GameSpeakEvents lastSpeak_1 = LastGameSpeak_476FF0();
                                if (field_17E_delayed_speak != MudAction::eUnknown_17)
                                {
                                    v18 = sub_477AF0(field_17E_delayed_speak);
                                    if (field_188_pTblEntry->field_0_sound == 14)
                                    {
                                        field_194_timer = sGnFrame_5C1B84 + 20;
                                    }
                                    else
                                    {
                                        field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                                    }
                                    field_178_sub_state2 = field_190_sub_state;
                                    goto LABEL_424;
                                }

                                if (lastSpeak_1 == GameSpeakEvents::eNone)
                                {
                                    if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
                                    {
                                        return field_190_sub_state;
                                    }
                                    field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                                    field_178_sub_state2 = field_190_sub_state;
                                    result = 21;
                                }
                                else
                                {
                                    field_178_sub_state2 = field_190_sub_state;
                                    field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                                    switch (lastSpeak_1)
                                    {
                                    case GameSpeakEvents::eFart_3:
                                        v18 = sub_477AF0(MudAction::eFart_6);
                                        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                                        {
                                            if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                                            {
                                                field_178_sub_state2 = 3;
                                            }
                                        }
                                        goto LABEL_425;

                                    case GameSpeakEvents::eHello_9:
                                    case GameSpeakEvents::eAllYa_23:
                                        goto LABEL_98;

                                    case GameSpeakEvents::eFollowMe_10:
                                        if (v44)
                                        {
                                            return 19;
                                        }
                                        result = sub_477AF0(MudAction::eFollowMe_1);
                                        field_178_sub_state2 = 2;
                                        if (result)
                                        {
                                            return result;
                                        }
                                        return field_190_sub_state;

                                    case GameSpeakEvents::eAnger_11:
                                    case GameSpeakEvents::eSorry_24:
                                        goto LABEL_176;

                                    case GameSpeakEvents::eWait_12:
                                        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                        {
                                            return 19;
                                        }
                                        field_16A_flags.Clear(Flags::eBit5);
                                        result = sub_477AF0(MudAction::eWait_2);
                                        field_178_sub_state2 = 7;
                                        if (result)
                                        {
                                            return result;
                                        }
                                        return field_190_sub_state;

                                    case GameSpeakEvents::eWork_21:
                                        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                        {
                                            return 19;
                                        }
                                        field_16A_flags.Clear(Flags::eBit5);
                                        field_182 = -1;
                                        if (!FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                                        {
                                            goto LABEL_112;
                                        }
                                        v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                        if (!v35)
                                        {
                                            goto LABEL_185;
                                        }
                                        break;

                                    case GameSpeakEvents::eStopIt_22:
                                        goto LABEL_175;
                                    default:
                                        return field_190_sub_state;
                                    }

                                    while (!v35->field_16A_flags.Get(Flags::eBit7))
                                    {
                                        v35 = static_cast<Mudokon*>(GetStackedSlapTarget_425290(v35->field_8_object_id, Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                        if (!v35)
                                        {
                                            goto LABEL_185;
                                        }
                                    }
                                LABEL_184:
                                    if (v35)
                                    {
                                        goto LABEL_114;
                                    }
                                LABEL_185:
                                    field_108_next_motion = Mud_Motion::TurnWheelBegin_57_474C00;
                                    if (field_16A_flags.Get(Flags::eBit3))
                                    {
                                        word_5C3010--;
                                        field_16A_flags.Set(Flags::eBit3);
                                    }
                                    field_16A_flags.Set(Flags::eBit7);
                                    field_18E_ai_state = Mud_AI_State::AI_State_3_47E0D0;
                                    result = 0;
                                }
                            }
                            else
                            {
                                field_194_timer = sGnFrame_5C1B84 + (unsigned __int16)Mudokon::StableDelay_477570();
                                if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                                {
                                    field_184_motion = 21;
                                    field_178_sub_state2 = 6;
                                    result = 10;
                                }
                                else if (field_16A_flags.Get(Flags::eBit4_blind) || !IsMotionUnknown_4730F0())
                                {
                                    field_184_motion = 1;
                                    field_178_sub_state2 = 5;
                                    result = 10;
                                }
                                else
                                {
                                    field_184_motion = 27;
                                    field_178_sub_state2 = 5;
                                    result = 10;
                                }
                            }
                        }
                        else
                        {
                            result = field_190_sub_state;
                            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                        }
                    }
                    else
                    {
                        result = field_190_sub_state;
                        field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                    }
                }
                else
                {
                    field_16A_flags.Clear(Flags::eBit5);
                    if (!field_10_resources_array.ItemAt(2))
                    {
                        goto LABEL_188;
                    }
                    field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                    result = 22;
                }
            }
        }
        return result;

    case 5u:
        if (field_106_current_motion != 0 || field_108_next_motion == Mud_Motion::Walking_1_4728B0)
        {
            goto LABEL_195;
        }
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return 4;
        }
        field_108_next_motion = Mud_Motion::Walking_1_4728B0;
    LABEL_195:
        if (field_106_current_motion == Mud_Motion::StandToKnockBack_46_4742A0)
        {
            Sound_475EC0(16);
            result = sub_477AF0(MudAction::eUnknown_11);
            field_17E_delayed_speak = MudAction::eUnknown_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (field_16A_flags.Get(Flags::eBit4_blind))
            {
                field_178_sub_state2 = 7;
                field_16A_flags.Clear(Flags::eBit5);
            }
            else
            {
                field_178_sub_state2 = 4;
            }
            if (result)
            {
                return result;
            }
        }

        if (field_106_current_motion != Mud_Motion::Walking_1_4728B0 && field_106_current_motion != Mud_Motion::Sneaking_27_473C40)
        {
            return field_190_sub_state;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            v65 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            v65 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        v67 = Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v65);
        if (field_16A_flags.Get(Flags::eBit4_blind))
        {
            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0)
                && !vIsFacingMe_4254A0(sActiveHero_5C1B68))
            {
                const int v68 = FP_GetExponent(field_B8_xpos) % 375;
                if (v68 > 10u && v68 < 362u)
                {
                    field_16A_flags.Clear(Flags::eBit5);
                    field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
                    return 7;
                }
            }
        }
        if (v67 && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 4;
        }

        if (sub_477880())
        {
            field_16A_flags.Set(Flags::eBit7);
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 4;
        }

        if (!vIsFacingMe_4254A0(sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 4;
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 4;
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 9;
        }

        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0 &&
            sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_23_RollLoop_453A90 &&
            field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            goto LABEL_231;
        }

        if (field_106_current_motion == Mud_Motion::Sneaking_27_473C40 || field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::Walking_1_4728B0;
        LABEL_231:
            if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550
                && field_106_current_motion == Mud_Motion::Walking_1_4728B0
                && !field_16A_flags.Get(Flags::eBit4_blind))
            {
                field_108_next_motion = Mud_Motion::Sneaking_27_473C40;
            }
            if (sActiveHero_5C1B68->field_106_current_motion != 1
                || field_106_current_motion != Mud_Motion::Sneaking_27_473C40)
            {
                return field_190_sub_state;
            }
            field_108_next_motion = Mud_Motion::Walking_1_4728B0;
            result = field_190_sub_state;
        }
        else
        {
            field_108_next_motion = Mud_Motion::Running_21_473720;
            result = 6;
        }
        return result;

    case 6u:
        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            goto LABEL_242;
        }
        if (!field_16A_flags.Get(Flags::eBit4_blind))
        {
            return 4;
        }
        field_108_next_motion = Mud_Motion::Running_21_473720;
    LABEL_242:
        v5 = 4;
        if (field_106_current_motion == Mud_Motion::StandToKnockBack_46_4742A0)
        {
            Sound_475EC0(16);
            result = sub_477AF0(MudAction::eUnknown_11);
            field_17E_delayed_speak = MudAction::eUnknown_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (field_16A_flags.Get(Flags::eBit4_blind))
            {
                field_178_sub_state2 = 7;
                field_16A_flags.Clear(Flags::eBit5);
            }
            else
            {
                field_178_sub_state2 = 4;
            }
            if (result)
            {
                return result;
            }
        }
        if (field_106_current_motion != Mud_Motion::Running_21_473720)
        {
            return field_190_sub_state;
        }

        FP v75;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            v75 = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            v75 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), v75 * FP_FromInteger(3)) && !field_16A_flags.Get(Flags::eBit4_blind))
        {
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            if (!(field_180_emo_tbl == Mud_Emotion::eWired_6))
            {
                return v5;
            }
            field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
            v80 = Math_NextRandom() < 20u;
            goto LABEL_264;
        }
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550)
        {
            if (field_180_emo_tbl != Mud_Emotion::eWired_6)
            {
                field_108_next_motion = Mud_Motion::Walking_1_4728B0;
                return 5;
            }
        LABEL_259:
            if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_25_RunSlideStop_451330 ||
                sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_0_Idle_44EEB0 ||
                sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_71_Knockback_455090 ||
                sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0)
            {
                field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
                field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
                v80 = Math_NextRandom() < 40u;
            LABEL_264:
                if (v80)
                {
                    return 4;
                }
                field_178_sub_state2 = 4;
                return 21;
            }
            goto LABEL_273;
        }
        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
        {
            goto LABEL_259;
        }
        if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0)
        {
            const FP v83 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            if (vIsObjNearby_4253B0(v83 * FP_FromInteger(4), sActiveHero_5C1B68))
            {
                field_108_next_motion = Mud_Motion::Walking_1_4728B0;
                return 5;
            }
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eWait_12)
        {
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::StandIdle_0_4724E0;
            return 9;
        }

    LABEL_273:
        // TODO: Ordering
        if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68) ||
            !vIsObj_GettingNear_425420(sActiveHero_5C1B68) && vIsFacingMe_4254A0(sActiveHero_5C1B68) ||
            field_16A_flags.Get(Flags::eBit4_blind))
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::RunSlideTurn_25_473AA0;
        return 8;

    case 7u:
    {
        if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(260) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags::eBit5);
                field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                return 22;
            }
        LABEL_125:
            field_16A_flags.Clear(Flags::eBit5);
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
            return 22;
        }
        if (field_106_current_motion)
        {
            field_17C = 0;
            field_124 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 60);
            return field_190_sub_state;
        }

        if (field_180_emo_tbl == Mud_Emotion::eUnknown_4)
        {
            field_16A_flags.Clear(Flags::eBit5);
            return 16;
        }

        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
        {
            field_16A_flags.Set(Flags::eBit5);
            return 4;
        }

        if ((++field_17C > 540 || Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0) && field_180_emo_tbl != Mud_Emotion::eWired_6)
        {
        LABEL_64:
            if (field_10_resources_array.ItemAt(2))
            {
                field_16A_flags.Clear(Flags::eBit5);
                field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                return 22;
            }
            goto LABEL_125;
        }

        bool bContinue = word_5C3010 <= 1;
        if (!bContinue)
        {
            auto pObj = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
            if (pObj)
            {
                if (vIsFacingMe_4254A0(pObj) || pObj->field_106_current_motion == Mud_Motion::TurnAroundStanding_2_472BF0)
                {
                    bContinue = true;
                }
            }
        }

        if (bContinue)
        {
            if (vIsFacingMe_4254A0(sActiveHero_5C1B68) || word_5C3010 > 1 && FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
            {
                if (field_17E_delayed_speak != MudAction::eUnknown_17)
                {
                    v18 = sub_477AF0(field_17E_delayed_speak);
                    if (field_188_pTblEntry->field_0_sound == 14)
                    {
                        field_194_timer = sGnFrame_5C1B84 + 20;
                        field_178_sub_state2 = field_190_sub_state;
                    }
                    else
                    {
                        field_178_sub_state2 = field_190_sub_state;
                        field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
                    }
                    goto LABEL_424;
                }
                GameSpeakEvents lastSpeak = LastGameSpeak_476FF0();
                if (lastSpeak == GameSpeakEvents::eNone
                    && ((lastSpeak = (GameSpeakEvents)field_182, lastSpeak == -1)
                        || field_180_emo_tbl != Mud_Emotion::eNormal_0 && field_180_emo_tbl != Mud_Emotion::eHappy_5 && field_180_emo_tbl != Mud_Emotion::eWired_6))
                {
                    if (field_180_emo_tbl != Mud_Emotion::eWired_6 || field_124 > (signed int)sGnFrame_5C1B84)
                    {
                        return field_190_sub_state;
                    }
                    field_194_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                    field_178_sub_state2 = field_190_sub_state;
                    result = 21;
                }
                else
                {
                    field_178_sub_state2 = field_190_sub_state;
                    field_194_timer = (unsigned __int16)StableDelay_477570() + sGnFrame_5C1B84 + 20;
                    switch (lastSpeak)
                    {
                    case GameSpeakEvents::eFart_3:
                        field_182 = -1;
                        v18 = sub_477AF0(MudAction::eFart_6);
                        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                        {
                            if (vOnSameYLevel_425520(sActiveHero_5C1B68))
                            {
                                field_178_sub_state2 = 3;
                            }
                        }
                        goto LABEL_425;

                    case GameSpeakEvents::eHello_9:
                    case GameSpeakEvents::eAllYa_23:
                        field_182 = -1;
                    LABEL_98:
                        result = sub_477AF0(MudAction::eHelloOrAllYa_0);
                        if (result)
                        {
                            return result;
                        }
                        return field_190_sub_state;

                    case GameSpeakEvents::eFollowMe_10:
                    {
                        const __int16 v26 = sub_477AF0(MudAction::eFollowMe_1);
                        if (field_188_pTblEntry->field_0_sound == 12 || field_188_pTblEntry->field_0_sound == 10)
                        {
                            field_16A_flags.Set(Flags::eBit5);
                            field_182 = -1;
                            // TODO: Wrong ??
                            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sActiveHero_5C1B68) ||
                                vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sActiveHero_5C1B68))
                            {
                                field_178_sub_state2 = 4;
                            }
                            else
                            {
                                field_178_sub_state2 = 2;
                            }
                        }
                        else
                        {
                            field_182 = lastSpeak;
                        }

                        if (!v26)
                        {
                            return field_190_sub_state;
                        }

                        return v26;
                    }

                    case GameSpeakEvents::eAnger_11:
                    case GameSpeakEvents::eSorry_24:
                    LABEL_176:
                        field_17C = 0;
                        return field_190_sub_state;

                    case GameSpeakEvents::eWait_12:
                        field_16A_flags.Clear(Flags::eBit5);
                        field_182 = -1;
                        field_184_motion = 43;
                        return 10;

                    case GameSpeakEvents::eWork_21:
                        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
                        {
                            return 19;
                        }
                        field_16A_flags.Clear(Flags::eBit5);
                        field_182 = -1;
                        if (FindWheel_4777B0(field_B8_xpos, field_BC_ypos))
                        {
                            v35 = static_cast<Mudokon*>(FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                            if (!v35)
                            {
                                goto LABEL_185;
                            }
                            while (!(v35->field_16A_flags.Get(Flags::eBit7)) && v35->field_106_current_motion != Mud_Motion::TurnWheelLoop_58_474CC0)
                            {
                                v35 = static_cast<Mudokon*>(GetStackedSlapTarget_425290(v35->field_8_object_id, Types::eMudokon_110, field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
                                if (!v35)
                                {
                                    goto LABEL_185;
                                }
                            }
                            goto LABEL_184;
                        }

                    LABEL_112:
                        if (FindObjectOfType_425180(Types::eLever_139, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
                        {
                            return 11;
                        }
                        if (FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                        {
                            return 11;
                        }
                        break;

                    case GameSpeakEvents::eStopIt_22:
                    LABEL_175:
                        v18 = sub_477AF0(MudAction::eUnknown_4);
                        field_194_timer = ((unsigned int)(unsigned __int16)StableDelay_477570() >> 1) + sGnFrame_5C1B84 + 20;
                        goto LABEL_425;
                    default:
                        return field_190_sub_state;
                    }

                LABEL_114:
                    if (field_10_resources_array.ItemAt(2))
                    {
                        field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
                        result = 22;
                    }
                    else
                    {
                    LABEL_188:
                        field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
                        result = 22;
                    }
                }
            }
            else
            {
                result = field_190_sub_state;
                field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
            }
        }
        else
        {
            result = field_190_sub_state;
            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
        }
        return result;
    }

    case 8u:
        if (field_106_current_motion == Mud_Motion::Running_21_473720)
        {
            return v5;
        }
        if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
        {
            return field_190_sub_state;
        }
        return 4;

    case 9u:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }
        field_178_sub_state2 = 7;
        field_194_timer = StableDelay_477570() + sGnFrame_5C1B84 + 20;
        return 20;

    case 10u:
        if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
        {
            goto LABEL_438;
        }
        field_108_next_motion = -1;

        if ((signed int)sGnFrame_5C1B84 <= field_194_timer)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = field_184_motion;
        return field_178_sub_state2;

    case 11u:
    {
        BaseGameObject* pLever = nullptr;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            pLever = FindObjectOfType_425180(Types::eLever_139, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
        }
        else
        {
            pLever = FindObjectOfType_425180(Types::eLever_139, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
        }

        if (!pLever)
        {
        LABEL_438:
            field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
            return field_190_sub_state;
        }

        if (field_106_current_motion == Mud_Motion::PullLever_10_473020)
        {
            field_108_next_motion = -1;
            abort();
            /*
            // TODO FIX ME
            ((void(*)(BaseAliveGameObject *, bool))pLever->field_0_VTbl->VBaseAliveGameObject.field_3C_vnull_408F90)(
                pLever,
                field_B8_xpos < pLever->field_B8_xpos);

            field_16A_flags.Clear(Flags::eBit5);
            v102 = -(field_10_resources_array.ItemAt(2) != 0);
            LOBYTE(v102) = v102 & ~3;
            field_108_next_motion = v102 + 15;
            result = 22;
            */
        }
        else
        {
            result = field_190_sub_state;
            field_108_next_motion = Mud_Motion::PullLever_10_473020;
        }
    }
        return result;

    case 12u:
    {
        if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
        {
            goto LABEL_419;
        }

        if (field_188_pTblEntry->field_2_next_motion != Mud_Motion::Slap_38_474AA0)
        {
            goto LABEL_317;
        }

        if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
        {
            if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
            {
                ToStand_4724A0();
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_108_next_motion = -1;
            }
            result = sub_477AF0(MudAction::eUnknown_4);
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (result)
            {
                return result;
            }
        }
        gnFrame = sGnFrame_5C1B84;
    LABEL_317:

        if (gnFrame <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_188_pTblEntry->field_0_sound != 23 || field_178_sub_state2 == 3)
        {
            if (field_188_pTblEntry->field_0_sound != 10 || (Math_NextRandom() >= 0x50u))
            {
                if (field_188_pTblEntry->field_0_sound != -1)
                {
                    Sound_475EC0(field_188_pTblEntry->field_0_sound);
                }
            }
            else
            {
                Sound_475EC0(8);
            }
        }
        else
        {
            if (Math_NextRandom() >= 80u)
            {
                Sound_475EC0(10);
            }
            else
            {
                Sound_475EC0(8);
            }
        }

        field_108_next_motion = field_188_pTblEntry->field_2_next_motion;
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }

        if (field_180_emo_tbl == Mud_Emotion::eSick_7)
        {
            field_18E_ai_state = Mud_AI_State::AI_Sick_9_47A910;
            return 0;
        }

        if (field_108_next_motion != Mud_Motion::Slap_38_474AA0)
        {
            return field_178_sub_state2;
        }

        BaseGameObject* v115 = nullptr;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                return 18;
            }
            v115 = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
        }
        else
        {
            if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                return 18;
            }
            v115 = FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5));
        }

        if (!v115)
        {
            return field_178_sub_state2;
        }
    }
        return 18;

    case 13u:
        StandingKnockBack_473190();
        //HIWORD(field_20_animation.field_4_flags) &= ~2u; // TODO: Check this
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        field_108_next_motion = -1;
        if (field_180_emo_tbl == Mud_Emotion::eWired_6)
        {
            return 21;
        }
        abort();
        /*
        // TODO FIX ME
        v129 = -(field_188_pTblEntry->field_2_next_motion != Mud_Motion::Slap_38_474AA0);
        LOBYTE(v129) = v129 & 0xFE;
        return v129 + 14;
        */

    case 14u:
        if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
        {
            goto LABEL_419;
        }
        if (LastGameSpeak_476FF0() == GameSpeakEvents::eStopIt_22)
        {
            if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
            {
                ToStand_4724A0();
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_108_next_motion = -1;
            }

            result = sub_477AF0(MudAction::eUnknown_4);
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (result)
            {
                return result;
            }
        }
        if ((signed int)sGnFrame_5C1B84 < field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                    || !FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5))
                    &&  FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                {
                LABEL_417:
                    field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                    field_194_timer = sGnFrame_5C1B84 + 15;
                    return 12;
                }
            }
        }
        else
        {
            if (!FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos + ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
            {
                if (FindObjectOfType_425180(Types::eType_Abe_69, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                {
                    goto LABEL_417;
                }
                if (!FindObjectOfType_425180(Types::eMudokon_110, ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)))
                {
                    if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale), field_BC_ypos - FP_FromInteger(5)))
                    {
                        goto LABEL_417;
                    }
                }
            }
        }
        return 12;

    case 15u:
        if (gnFrame <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_188_pTblEntry->field_0_sound != -1)
        {
            Sound_475EC0(field_188_pTblEntry->field_0_sound);
        }

        field_108_next_motion = field_188_pTblEntry->field_2_next_motion;
        if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
        {
            field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
        }
        if (field_182 != 10)
        {
            return field_178_sub_state2;
        }

        field_16A_flags.Set(Flags::eBit5);
        field_182 = -1;
        return 4;

    case 16u:
        if (field_106_current_motion != Mud_Motion::SlapOwnHead_56_4727D0)
        {
            field_108_next_motion = Mud_Motion::SlapOwnHead_56_4727D0;
        }
        if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22 && field_17E_delayed_speak != MudAction::eSorry_8)
        {
            return field_190_sub_state;
        }
        result = sub_477AF0(MudAction::eUnknown_4);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (!result)
        {
            return field_190_sub_state;
        }
        return result;

    case 17u:
        if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
        {
        LABEL_419:
            v18 = sub_477AF0(field_17E_delayed_speak);
            if (field_188_pTblEntry->field_0_sound == 14)
            {
                field_194_timer = sGnFrame_5C1B84 + 20;
                goto LABEL_423;
            }
            v126 = (unsigned __int16)(StableDelay_477570() + sGnFrame_5C1B84 + 20);
            goto LABEL_422;
        }

        if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
        {
            goto LABEL_442;
        }
        if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
        {
            ToStand_4724A0();
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_108_next_motion = -1;
        }
        result = sub_477AF0(MudAction::eUnknown_4);
        field_178_sub_state2 = 7;
        field_17E_delayed_speak = MudAction::eUnknown_17;
        field_194_timer = sGnFrame_5C1B84 + 20;
        if (result)
        {
            return result;
        }
    LABEL_442:
        if (field_106_current_motion)
        {
            return field_190_sub_state;
        }
        field_108_next_motion = Mud_Motion::Slap_38_474AA0;
    LABEL_364:
        result = 7;
        return result;

    case 18u:
        if (field_17E_delayed_speak == MudAction::eMudDied_14 || field_17E_delayed_speak == MudAction::eUnknown_9)
        {
            v18 = sub_477AF0(field_17E_delayed_speak);
            if (field_188_pTblEntry->field_0_sound == 14)
            {
                v126 = (short)(sGnFrame_5C1B84 + 20);
            LABEL_422:
                field_194_timer = v126;
            }
            else
            {
                field_194_timer = (unsigned __int16)Mudokon::StableDelay_477570() + sGnFrame_5C1B84 + 20;
            }
        LABEL_423:
            field_178_sub_state2 = 7;
        LABEL_424:
            field_17E_delayed_speak = MudAction::eUnknown_17;
        LABEL_425:
            if (!v18)
            {
                return field_190_sub_state;
            }
            result = v18;
        }
        else
        {
            if (LastGameSpeak_476FF0() != GameSpeakEvents::eStopIt_22)
            {
                goto LABEL_443;
            }
            if (field_106_current_motion == Mud_Motion::Slap_38_474AA0 || field_108_next_motion == Mud_Motion::Slap_38_474AA0)
            {
                ToStand_4724A0();
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_108_next_motion = -1;
            }
            result = sub_477AF0(MudAction::eUnknown_4);
            field_178_sub_state2 = 7;
            field_17E_delayed_speak = MudAction::eUnknown_17;
            field_194_timer = sGnFrame_5C1B84 + 20;
            if (!result)
            {
            LABEL_443:
                if (field_106_current_motion != Mud_Motion::StandIdle_0_4724E0)
                {
                    return field_190_sub_state;
                }
                field_108_next_motion = Mud_Motion::TurnAroundStanding_2_472BF0;
                result = 17;
            }
        }
        return result;

    case 19u:
        if (gnFrame <= field_194_timer)
        {
            return field_190_sub_state;
        }

        if (field_180_emo_tbl == Mud_Emotion::eAngry_1 || field_180_emo_tbl == Mud_Emotion::eUnknown_2)
        {
            Sound_475EC0(21);
        }
        else if (field_180_emo_tbl == Mud_Emotion::eSad_3 || field_180_emo_tbl == Mud_Emotion::eUnknown_4)
        {
            Sound_475EC0(22);
        }
        else
        {
            Sound_475EC0(13);
        }

        field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
        return 4;

    case 20u:
        if (gnFrame <= field_194_timer)
        {
            return field_190_sub_state;
        }
        Sound_475EC0(12);
        result = field_178_sub_state2;
        field_108_next_motion = Mud_Motion::Speak_Generic_3_472FA0;
        return result;

    case 21u:
        if (field_106_current_motion)
        {
            return 21;
        }

        if (gnFrame <= field_194_timer)
        {
            return field_190_sub_state;
        }

        field_16A_flags.Clear(Flags::eBit12);

        if (Math_NextRandom() >= 90u)
        {
            Sound_475EC0(10);
        }
        else
        {
            Sound_475EC0(8);
        }

        field_108_next_motion = Mud_Motion::Speak_Generic_6_472FA0;
        return field_178_sub_state2;

    case 22u:
        if (field_10_resources_array.ItemAt(2))
        {
            field_108_next_motion = Mud_Motion::Chisel_11_4732D0;
        }
        else
        {
            field_108_next_motion = Mud_Motion::CrouchIdle_15_474040;
        }

        if (field_106_current_motion != Mud_Motion::CrouchIdle_15_474040 && field_106_current_motion != Mud_Motion::Chisel_11_4732D0)
        {
            return field_190_sub_state;
        }

        if (field_16A_flags.Get(Flags::eBit3))
        {
            word_5C3010--;
            field_16A_flags.Clear(Flags::eBit3);
        }

        if (field_10_resources_array.ItemAt(2))
        {
            field_18E_ai_state = Mud_AI_State::AI_Chisel_1_47C5F0;
        }
        else
        {
            field_18E_ai_state = Mud_AI_State::AI_Scrub_2_47D270;
        }

        field_182 = -1;
        return 0;

    default:
        return field_190_sub_state;
    }
}

__int16 Mudokon::AI_ShrivelDeath_5_4714A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_HelloAlerted_6_47A560()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_FallAndSmackDeath_7_471600()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_AngryWorker_8_47E910()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::AI_Sick_9_47A910()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::StandIdle_0_4724E0()
{
    ToFalling_472320();

    field_F8 = field_BC_ypos;

    if (field_100_pCollisionLine)
    {
        if ((field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36) && field_110_id == -1)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    switch (field_108_next_motion)
    {
    case Mud_Motion::Walking_1_4728B0:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
        }
        field_106_current_motion = Mud_Motion::StandToWalk_7_472AB0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Sneaking_27_473C40:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
        }
        field_106_current_motion = Mud_Motion::StandToSneaking_32_473F80;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Running_21_473720:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }

        field_106_current_motion = Mud_Motion::WalkToRun_37_4749A0;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Chanting_50_473040:
        SND_SEQ_PlaySeq_4CA960(11, 0, 1);
        field_106_current_motion = Mud_Motion::Chanting_50_473040;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::CrouchIdle_15_474040:
        field_106_current_motion = Mud_Motion::StandToCrouch_17_474120;
        field_108_next_motion = -1;
        break;

    case Mud_Motion::Chisel_11_4732D0:
    case Mud_Motion::CrouchScrub_14_473560:
        field_106_current_motion = Mud_Motion::StandToCrouch_17_474120;
        break;

    default:
        if (field_108_next_motion != -1)
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
        break;
    }
}

void Mudokon::Walking_1_4728B0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnAroundStanding_2_472BF0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Speak_Generic_472FA0()
{
    ToFalling_472320();
    SetPal_4772D0(field_180_emo_tbl);

    if (field_16A_flags.Get(Flags::eBit12))
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_16A_flags.Set(Flags::eBit12);
        SetPal_4772D0(Mud_Emotion::eNormal_0);
        ToStand_4724A0();
    }
}

void Mudokon::StandToWalk_7_472AB0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkingToStand_8_472B30()
{
    NOT_IMPLEMENTED();
}

void Mudokon::jWalkingToStand_8_472BD0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::PullLever_10_473020()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Chisel_11_4732D0()
{
    ToFalling_472320();

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sGnFrame_5C1B84 & 1)
            {
                SFX_Play_46FA90(91, 0, field_CC_sprite_scale);

                auto pSpark = alive_new<Spark>();
                if (pSpark)
                {
                    FP sparkY = {};
                    FP sparkX = {};
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        sparkY = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(3));
                        sparkX = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(18));
                    }
                    else
                    {
                        sparkY = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(3));
                        sparkX = (field_CC_sprite_scale * FP_FromInteger(18)) + field_B8_xpos;
                    }

                    pSpark->ctor_4CBBB0(
                        sparkX + FP_FromInteger(field_DA_xOffset),
                        sparkY,
                        field_CC_sprite_scale,
                        9,
                        0,
                        255,
                        0);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        switch (field_108_next_motion)
        {
        case Mud_Motion::StandIdle_0_4724E0:
            field_106_current_motion = Mud_Motion::StopChisel_13_473530;
            return;

        case Mud_Motion::CrouchIdle_15_474040:
            field_106_current_motion = Mud_Motion::StopChisel_13_473530;
            break;

        case -1:
            return;

        default:
            field_106_current_motion = field_108_next_motion;
            break;
        }

        field_108_next_motion = -1;
    }
}

void Mudokon::StartChisel_12_473500()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::Chisel_11_4732D0;
    }
}

void Mudokon::StopChisel_13_473530()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
}

void Mudokon::CrouchScrub_14_473560()
{
    NOT_IMPLEMENTED();
}

void Mudokon::CrouchIdle_15_474040()
{
    ToFalling_472320();

    if (field_108_next_motion == Mud_Motion::Duck_53_474A40)
    {
        field_106_current_motion = Mud_Motion::ToDuck_52_474A20;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::StandIdle_0_4724E0)
    {
        field_106_current_motion = Mud_Motion::CrouchToStand_18_474150;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion == Mud_Motion::Chisel_11_4732D0)
    {
        field_106_current_motion = Mud_Motion::StartChisel_12_473500;
        field_108_next_motion = -1;
    }
    else if (field_108_next_motion != -1)
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Mudokon::CrouchTurn_16_4740E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToCrouch_17_474120()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = Mud_Motion::CrouchIdle_15_474040;
    }
}

void Mudokon::CrouchToStand_18_474150()
{
    ToFalling_472320();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4724A0();
    }
}

void Mudokon::StandingToRun1_19_473600()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandingToRun2_20_4736D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Running_21_473720()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunToWalk1_22_4738E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunToWalk2_23_4739B0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunSlideStop_24_473A00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunSlideTurn_25_473AA0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::RunTurn_26_473BB0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Sneaking_27_473C40()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToSneak1_28_473D60()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToWalk_29_473E20()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToSneak2_30_473EE0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToWalk2_31_473F30()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToSneaking_32_473F80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToStand1_33_473FF0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SneakingToStand2_34_474020()
{
    NOT_IMPLEMENTED();
}

void Mudokon::JumpStart_35_474460()
{
    NOT_IMPLEMENTED();
}

void Mudokon::JumpMid_36_474570()
{
    NOT_IMPLEMENTED();
}

void Mudokon::WalkToRun_37_4749A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Slap_38_474AA0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StartHoistJumpUp_39_4748E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HoistFallToFloor_40_474780()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HitFloorStanding1_41_474960()
{
    NOT_IMPLEMENTED();
}

void Mudokon::HitFloorStanding2_42_4743F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToDunno_43_472790()
{
    NOT_IMPLEMENTED();
}

void Mudokon::DunnoToStand_44_4727B0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::KnockForward_45_474180()
{
    NOT_IMPLEMENTED();
}

void Mudokon::StandToKnockBack_46_4742A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::KnockBackToStand_47_474380()
{
    NOT_IMPLEMENTED();
}

void Mudokon::FallLedgeBegin_48_4743C0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Fall_49_472C60()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Chanting_50_473040()
{
    NOT_IMPLEMENTED();
}

void Mudokon::ToChant_51_4730D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::ToDuck_52_474A20()
{
    NOT_IMPLEMENTED();
}

void Mudokon::Duck_53_474A40()
{
    NOT_IMPLEMENTED();
}

void Mudokon::DuckToCrouch_54_474A80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::DuckKnockBack_55_474250()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SlapOwnHead_56_4727D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelBegin_57_474C00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelLoop_58_474CC0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::TurnWheelEnd_59_474D30()
{
    NOT_IMPLEMENTED();
}

__int16 Mudokon::StableDelay_477570()
{
    if (word_5C3010 >= ALIVE_COUNTOF(kDelayTable_55CF7C))
    {
        word_5C3010 = ALIVE_COUNTOF(kDelayTable_55CF7C);
    }

    if (sDelayIdx_5C3014 >= word_5C3010)
    {
        sDelayIdx_5C3014 = 0;
    }

    return kDelayTable_55CF7C[sDelayIdx_5C3014++];
}

__int16 Mudokon::CheckForPortal_4775E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

GameSpeakEvents Mudokon::LastGameSpeak_476FF0()
{
    NOT_IMPLEMENTED();

    __int16 actualEvent; // si

    const int lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
    if (field_140 == lastEventIdx)
    {
        // TODO -2 is for ???
        // -1 = -1 - -2 = 1
        // 0 -2 = -2
        actualEvent = (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone) - 2;
    }
    else
    {
        field_140 = lastEventIdx;
        actualEvent = pEventSystem_5BC11C->field_20_last_event;
    }

    // Not valid if not in the same camera
    if (Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0)
    {
        return GameSpeakEvents::eNone;
    }
    
    // Look out works on any scale
    if (actualEvent == GameSpeakEvents::eLookOut_6)
    {
        return GameSpeakEvents::eLookOut_6;
    }

    // Check in valid range and on same scale
    if (actualEvent < 1 || actualEvent > GameSpeakEvents::eSorry_24 || sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        return (GameSpeakEvents)actualEvent;
    }

    return GameSpeakEvents::eNone;
}

__int16 Mudokon::LaughingGasInCurrentScreen_4774A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::Sound_475EC0(__int16 /*idx*/)
{
    NOT_IMPLEMENTED();
}

__int16 Mudokon::CanRespond_4770B0()
{
    NOT_IMPLEMENTED();
    return 1;
}

BYTE** Mudokon::AnimBlockForMotion_474DC0(short motion)
{
    if (motion < Mud_Motion::PullLever_10_473020)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else if (motion < Mud_Motion::Chisel_11_4732D0)
    {
        if (!field_10_resources_array.ItemAt(1))
        {
            field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbepullResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(1);
    }
    else if (motion < Mud_Motion::CrouchScrub_14_473560)
    {
        return field_10_resources_array.ItemAt(2);
    }
    else if (motion < Mud_Motion::CrouchIdle_15_474040)
    {
        return field_10_resources_array.ItemAt(3);
    }
    else if (motion < Mud_Motion::KnockForward_45_474180)
    {
        return field_10_resources_array.ItemAt(4);
    }
    else if (motion < Mud_Motion::StandToKnockBack_46_4742A0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    else if (motion < Mud_Motion::FallLedgeBegin_48_4743C0)
    {
        return field_10_resources_array.ItemAt(6);
    }
    else if (motion < Mud_Motion::Chanting_50_473040)
    {
        return field_10_resources_array.ItemAt(7);
    }
    else if (motion < Mud_Motion::ToDuck_52_474A20)
    {
        return field_10_resources_array.ItemAt(8);
    }
    else if (motion < Mud_Motion::DuckKnockBack_55_474250)
    {
        return field_10_resources_array.ItemAt(9);
    }
    else if (motion < Mud_Motion::SlapOwnHead_56_4727D0)
    {
        return field_10_resources_array.ItemAt(10);
    }
    else if (motion < Mud_Motion::TurnWheelBegin_57_474C00)
    {
        return field_10_resources_array.ItemAt(11);
    }
    else if (motion < 60) // MAX ??
    {
        if (!field_10_resources_array.ItemAt(12))
        {
            field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeworkResID, TRUE, FALSE));
        }
        return field_10_resources_array.ItemAt(12);
    }
    else
    {
        LOG_ERROR("Out of bounds ??");
        return nullptr;
    }
}

void Mudokon::ToFalling_472320()
{
    if (!field_100_pCollisionLine)
    {
        vOnTrapDoorOpen_4081F0();
        field_106_current_motion = Mud_Motion::Fall_49_472C60;
    }
}

void Mudokon::ToStand_4724A0()
{
    field_134 = 0;
    field_138 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = Mud_Motion::StandIdle_0_4724E0;
}

__int16 Mudokon::FindWheel_4777B0(FP /*xpos*/, FP /*ypos*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Mudokon::sub_477AF0(MudAction action)
{
    field_188_pTblEntry = ResponseTo_471730(field_180_emo_tbl, action);
    if (field_188_pTblEntry->field_4_emo_tbl != Mud_Emotion::eSad_3 || field_16A_flags.Get(Flags::eBit11_get_depressed))
    {
        field_180_emo_tbl = field_188_pTblEntry->field_4_emo_tbl;
    }
    return field_188_pTblEntry->field_6_sub_state;

}

__int16 Mudokon::sub_477880()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Mudokon::StandingKnockBack_473190()
{
    NOT_IMPLEMENTED();
}

// TODO: Abe sneaking or walking check ??
__int16 Mudokon::IsMotionUnknown_4730F0()
{
    return 
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_40_SneakLoop_450550 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_45_SneakBegin_4507A0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_41_450250 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_43_450380 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_42_4503D0 ||
        sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_44_450500;
}

const struct MudEmotionTableEntry* CC Mudokon::ResponseTo_471730(Mud_Emotion emotion, MudAction action)
{
    return &kMudEmoTable_55C790.field_0_data[static_cast<int>(emotion)][static_cast<int>(action)];
}