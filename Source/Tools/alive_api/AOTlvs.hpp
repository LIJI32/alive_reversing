#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Door.hpp"
#include "../AliveLibAO/Switch.hpp"
#include "../AliveLibAO/DoorLight.hpp"
#include "../AliveLibAO/ElectricWall.hpp"
#include "../AliveLibAO/Well.hpp"
#include "../AliveLibAO/Slig.hpp"

#define CTOR_AO(className, objectTypeName, tlvType)  className(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO(tlvType, objectTypeName, pTlv)

namespace AO
{
    struct Path_ContinueZone : public Path_TLV
    {
        int field_10_zone_number;
    };

    struct Path_StartController : public Path_TLV
    {
        // No fields
    };

    struct Path_InvisibleZone : public Path_TLV
    {
        // No fields
    };

    struct Path_DeathDrop : public Path_TLV
    {
        // No fields
    };
}

namespace AOTlvs
{
    struct Path_Door : public TlvObjectBaseAO<AO::Path_Door>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::DoorStates>("Enum_DoorStates",
                {
                    {AO::DoorStates::eOpen_0, "Open"},
                    {AO::DoorStates::eClosed_1, "Closed"},
                    {AO::DoorStates::eOpening_2, "Opening"},
                    {AO::DoorStates::eClosing_3, "Closing"},
                });
        }

        CTOR_AO(Path_Door, "Door", AO::TlvTypes::Door_6)
        {
            ADD("Level", mTlv.field_18_level);
            ADD("Path", mTlv.field_1A_path);
            ADD("Camera", mTlv.field_1C_camera);
            ADD("Scale", mTlv.field_1E_scale);
            ADD("DoorNumber", mTlv.field_20_door_number);
            ADD("Id", mTlv.field_22_id);
            ADD("TargetDoorNumber", mTlv.field_24_target_door_number);
            ADD("StartState", mTlv.field_26_start_state);
            ADD("DoorClosed", mTlv.field_28_door_closed);
            ADD("Hub1", mTlv.field_2A_hub1);
            ADD("Hub2", mTlv.field_2A_hub2);
            ADD("Hub3", mTlv.field_2A_hub3);
            ADD("Hub4", mTlv.field_2A_hub4);
            ADD("Hub5", mTlv.field_2A_hub5);
            ADD("Hub6", mTlv.field_2A_hub6);
            ADD("Hub7", mTlv.field_2A_hub7);
            ADD("Hub8", mTlv.field_2A_hub8);
            ADD("WipeEffect", mTlv.field_3A_wipe_effect);
            ADD("MovieNumber", mTlv.field_3C_movie_number);
            ADD("XOffset", mTlv.field_3E_x_offset);
            ADD("YOffset", mTlv.field_40_y_offset);
            ADD("WipeXOrg", mTlv.field_42_wipe_x_org);
            ADD("WipeYOrg", mTlv.field_44_wipe_y_org);
            ADD("AbeDirection", mTlv.field_46_abe_direction);
        }
    };

    struct Path_ContinuePoint : public TlvObjectBaseAO<AO::Path_ContinuePoint>
    {
        CTOR_AO(Path_ContinuePoint, "ContinuePoint",  AO::TlvTypes::ContinuePoint_0)
        {
            ADD("ZoneNumber", mTlv.field_18_zone_number);
            ADD("ClearFromId", mTlv.field_1A_clear_from_id);
            ADD("ClearToId", mTlv.field_1C_clear_to_id);
            ADD("ElumRestarts", mTlv.field_1E_elum_restarts);
            ADD("AbeSpawnDirection", mTlv.field_20_abe_direction);
        }
    };

    struct Path_Hoist : public TlvObjectBaseAO<AO::Path_Hoist>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::Path_Hoist::Type>("Enum_HoistType",
                {
                    {AO::Path_Hoist::Type::eNextEdge, "next_edge"},
                    {AO::Path_Hoist::Type::eNextFloor, "next_floor"},
                    {AO::Path_Hoist::Type::eOffScreen, "off_screen"},
                });

            types.AddEnum<AO::Path_Hoist::EdgeType>("Enum_HoistEdgeType",
                {
                    {AO::Path_Hoist::EdgeType::eBoth, "both"},
                    {AO::Path_Hoist::EdgeType::eLeft, "left"},
                    {AO::Path_Hoist::EdgeType::eRight, "right"},
                });
        }

        CTOR_AO(Path_Hoist, "Hoist", AO::TlvTypes::Hoist_3)
        {
            ADD("HoistType", mTlv.field_18_hoist_type);
            ADD("HoistEdgeType", mTlv.field_1A_edge_type);
            ADD("Id", mTlv.field_1C_id);
        }
    };

    struct Path_Change : public TlvObjectBaseAO<AO::Path_ChangeTLV>
    {
        CTOR_AO(Path_Change, "PathTransition", AO::TlvTypes::PathTransition_1)
        {
            ADD("Level", mTlv.field_18_level);
            ADD("HoistEdgeType", mTlv.field_1A_path);
            ADD("Camera", mTlv.field_1C_camera);
            ADD("Movie", mTlv.field_1E_movie); // TODO: Enum
            ADD("Wipe", mTlv.field_20_wipe); // TODO: Enum
            ADD("Scale", mTlv.field_22_scale); // TODO: Enum
        }
    };

    struct Path_Switch : public TlvObjectBaseAO<AO::Path_Switch>
    {
        CTOR_AO(Path_Switch, "Switch", AO::TlvTypes::Switch_26)
        {
            ADD("TriggerObject", mTlv.field_18_trigger_object);
            ADD("TriggerAction", mTlv.field_1A_trigger_object_action);
            ADD("Scale", mTlv.field_1C_scale);
            ADD("OnSound", mTlv.field_1E_on_sound); // TODO: Enum
            ADD("OffSound", mTlv.field_20_off_sound); // TODO: Enum
            ADD("SoundDirection", mTlv.field_22_sound_direction); // TODO: Enum
        }
    };

    struct Path_LightEffect : public TlvObjectBaseAO<AO::Path_LightEffect>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::Path_LightEffect::Type>("Enum_LightType",
                {
                    {AO::Path_LightEffect::Type::Star_0, "Star"},
                    {AO::Path_LightEffect::Type::RedGlow_1, "RedGlow"},
                    {AO::Path_LightEffect::Type::GreenGlow_2, "GreenGlow"},
                    {AO::Path_LightEffect::Type::FlintGlow_3, "FlintGlow"},
                    {AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4, "RedGreenDoorLight"},
                    {AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5, "RedGreenHubLight"},
                });
        }

        CTOR_AO(Path_LightEffect, "LightEffect", AO::TlvTypes::LightEffect_106)
        {
            ADD("Type", mTlv.field_18_type);
            ADD("Size", mTlv.field_1A_size);
            ADD("Id", mTlv.field_1C_id);
            ADD("FlipX", mTlv.field_1E_flip_x);
        }
    };

    struct Path_ElectricWall : public TlvObjectBaseAO<AO::Path_ElectricWall>
    {
        CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
        {
            ADD("Scale", mTlv.field_18_scale);
            ADD("Id", mTlv.field_1A_id);
            ADD("State", mTlv.field_1C_start_state); // TODO: Enum
        }
    };

    struct Path_ContinueZone : public TlvObjectBaseAO<AO::Path_ContinueZone>
    {
        CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
        {
            ADD("ZoneNumber", mTlv.field_10_zone_number);
        }
    };

    struct Path_StartController : public TlvObjectBaseAO<AO::Path_StartController>
    {
        CTOR_AO(Path_StartController, "StartController", AO::TlvTypes::StartController_28)
        {
            // No fields
        }
    };

    struct Path_Edge : public TlvObjectBaseAO<AO::Path_Edge>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::Path_Edge::Type>("Enum_EdgeType",
                {
                    {AO::Path_Edge::Type::eLeft, "left"},
                    {AO::Path_Edge::Type::eRight, "right"},
                    {AO::Path_Edge::Type::eBoth, "both"},
                });
        }

        CTOR_AO(Path_Edge, "Edge", AO::TlvTypes::Edge_4)
        {
            ADD("type", mTlv.field_18_type);
            ADD("can_grab", mTlv.field_1A_can_grab);
        }
    };

    struct Path_WellLocal : public TlvObjectBaseAO<AO::Path_WellLocal>
    {
        CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
        {
            ADD("on_dx", mTlv.field_28_on_dx);
            ADD("on_dy", mTlv.field_2A_on_dy);
            ADD("emit_leaves", mTlv.field_2C_emit_leaves);
            ADD("leaf_x", mTlv.field_2E_leaf_x);
            ADD("leaf_y", mTlv.field_30_leaf_y);
        }
    };

    struct Path_WellExpress : public TlvObjectBaseAO<AO::Path_WellExpress>
    {
        CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
        {
            ADD("off_camera", mTlv.field_28_off_camera);
            ADD("off_well_id", mTlv.field_2A_off_well_id);
            ADD("on_level", mTlv.field_2C_on_level);
            ADD("on_path", mTlv.field_2E_on_path);
            ADD("on_camera", mTlv.field_30_on_camera);
            ADD("on_well_id", mTlv.field_32_on_well_id);
            ADD("emit_leaves", mTlv.field_34_emit_leaves);
            ADD("leaf_x", mTlv.field_36_leaf_x);
            ADD("leaf_y", mTlv.field_38_leaf_y);
            ADD("movie_id", mTlv.field_3A_movie_id);
        }
    };

    struct Path_InvisibleZone : public TlvObjectBaseAO<AO::Path_InvisibleZone>
    {
        CTOR_AO(Path_InvisibleZone, "InvisibleZone", AO::TlvTypes::InvisibleZone_58)
        {
            // No fields
        }
    };

    struct Path_EnemyStopper : public TlvObjectBaseAO<AO::Path_EnemyStopper>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
                {
                    {AO::Path_EnemyStopper::StopDirection::Left_0, "left"},
                    {AO::Path_EnemyStopper::StopDirection::Right_1, "right"},
                    {AO::Path_EnemyStopper::StopDirection::Both_2, "both"},
                });
        }

        CTOR_AO(Path_EnemyStopper, "EnemyStopper", AO::TlvTypes::EnemyStopper_79)
        {
            ADD("direction", mTlv.field_18_direction);
            ADD("id", mTlv.field_1A_id);
        }
    };

    struct Path_Slig : public TlvObjectBaseAO<AO::Path_Slig>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::Path_Slig::StartState>("Enum_SligStartState",
                {
                    {AO::Path_Slig::StartState::Listening_0, "listening"},
                    {AO::Path_Slig::StartState::Paused_1, "paused"},
                    {AO::Path_Slig::StartState::Sleeping_2, "sleeping"},
                    {AO::Path_Slig::StartState::Chase_3, "chase"},
                    {AO::Path_Slig::StartState::GameEnder_4, "game_ender"},
                    {AO::Path_Slig::StartState::Paused_5, "paused"},
                });
        }

        CTOR_AO(Path_Slig, "Slig", AO::TlvTypes::Slig_24)
        {
            ADD("scale", mTlv.field_18_scale);
            ADD("start_state", mTlv.field_1A_start_state);
            ADD("pause_time", mTlv.field_1C_pause_time);
            ADD("pause_left_min", mTlv.field_1E_pause_left_min);
            ADD("pause_left_max", mTlv.field_20_pause_left_max);
            ADD("pause_right_min", mTlv.field_22_pause_right_min);
            ADD("pause_right_max", mTlv.field_24_pause_right_max);
            ADD("chal_type", mTlv.field_26_chal_type);
            ADD("chal_time", mTlv.field_28_chal_time);
            ADD("number_of_times_to_shoot", mTlv.field_2A_number_of_times_to_shoot);
            ADD("unknown", mTlv.field_2C_unknown);
            ADD("code1", mTlv.field_2E_code1);
            ADD("code2", mTlv.field_30_code2);
            ADD("chase_abe", mTlv.field_32_chase_abe);
            ADD("start_direction", mTlv.field_34_start_direction);
            ADD("panic_timeout", mTlv.field_36_panic_timeout);
            ADD("num_panic_sounds", mTlv.field_38_num_panic_sounds);
            ADD("panic_sound_timeout", mTlv.field_3A_panic_sound_timeout);
            ADD("stop_chase_delay", mTlv.field_3C_stop_chase_delay);
            ADD("time_to_wait_before_chase", mTlv.field_3E_time_to_wait_before_chase);
            ADD("slig_id", mTlv.field_40_slig_id);
            ADD("listen_time", mTlv.field_42_listen_time);
            ADD("percent_say_what", mTlv.field_44_percent_say_what);
            ADD("percent_beat_mud", mTlv.field_46_percent_beat_mud);
            ADD("talk_to_abe", mTlv.field_48_talk_to_abe);
            ADD("dont_shoot", mTlv.field_4A_dont_shoot);
            ADD("z_shoot_delay", mTlv.field_4C_z_shoot_delay);
            ADD("stay_awake", mTlv.field_4E_stay_awake);
            ADD("disable_resources", mTlv.field_50_disable_resources.Raw().all);
            ADD("noise_wake_up_distance", mTlv.field_52_noise_wake_up_distance);
            ADD("id", mTlv.field_54_id);
        }
    };

    struct Path_DeathDrop : public TlvObjectBaseAO<AO::Path_DeathDrop>
    {
        CTOR_AO(Path_DeathDrop, "DeathDrop", AO::TlvTypes::DeathDrop_5)
        {
            // No fields
        }
    };

    struct Path_SligLeftBound : public TlvObjectBaseAO<AO::Path_SligLeftBound>
    {
        CTOR_AO(Path_SligLeftBound, "SligLeftBound", AO::TlvTypes::eSligBoundLeft_57)
        {
            ADD("id", mTlv.field_18_slig_id);
            ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligRightBound : public TlvObjectBaseAO<AO::Path_SligRightBound>
    {
        CTOR_AO(Path_SligRightBound, "SligRightBound", AO::TlvTypes::eSligBoundRight_76)
        {
            ADD("id", mTlv.field_18_slig_id);
            ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligPersist : public TlvObjectBaseAO<AO::Path_SligPersist>
    {
        CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
        {
            ADD("id", mTlv.field_18_slig_id);
            ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };
}
