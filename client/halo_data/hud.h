#pragma once

#include "tag_data.h"
#include "../../math/data_types.h"

struct AnchorOffset {
    short x;
    short y;
};

enum HUDAnchor : uint16_t {
    ANCHOR_TOP_LEFT = 0,
    ANCHOR_TOP_RIGHT,
    ANCHOR_BOTTOM_LEFT,
    ANCHOR_BOTTOM_RIGHT,
    ANCHOR_CENTER
};

enum HUDMapType : uint16_t {
    MAP_TYPE_ANY = 0,
    MAP_TYPE_SOLO,
    MAP_TYPE_MULTIPLAYER
};

template<class T>
struct HUDInterfaceWarningSound {
    HaloTagDependency sound;
    T latched_to;
    float scale;
    PAD_CHARS(0x20);
};


struct HUDElementScalingFlags {
    uint16_t dont_scale_offset : 1;
    uint16_t dont_scale_size : 1;
    uint16_t use_high_resolution_scale : 1;
    PAD_BITS(uint16_t, 13);
};

struct HUDElementPosition {
    AnchorOffset anchor_offset;
    float width_scale;
    float height_scale;
    HUDElementScalingFlags scaling_flags;
    PAD_CHARS(0x16);
};

struct HUDElementColors {
    ColorByte default_color;
    ColorByte flashing_color;
    float flash_period;
    float flash_delay;
    uint16_t number_of_flashes;

    struct {
        uint16_t reverse_default_flashing_colors : 1;
        PAD_BITS(uint16_t, 15);
    } flash_flags;

    float flash_length;
    ColorByte disabled_color;
    PAD_CHARS(0x4);
};

struct HUDMeterColors {
    ColorByte color_at_meter_minimum;
    ColorByte color_at_meter_maximum;
    ColorByte flash_color;
    ColorByte empty_color;
    struct {
        uint8_t use_min_max_for_state_changes : 1;
        uint8_t interpolate_between_min_max_flash_colors_as_state_changes : 1;
        uint8_t interpolate_color_along_HSV_space : 1;
        uint8_t more_colors_for_HSV_interpolation : 1;
        uint8_t invert_interpolation : 1;
        PAD_BITS(uint8_t, 3);
    } flags;
    char minimum_meter_value;
    uint16_t sequence_index;
    char alpha_multiplier;
    char alpha_bias;
    char value_scale;
    PAD_CHARS(1);
    float opacity;
    float translucency;
    ColorByte disabled_color;
};

static_assert(sizeof(HUDMeterColors) == 0x24);

struct HUDNumberElementFlags {
    uint8_t show_leading_zeros : 1;
    uint8_t only_show_when_zoomed : 1;
    uint8_t show_meters_unit : 1;
    PAD_BITS(uint8_t, 5);
};

struct HUDOverlay {
    HUDElementPosition position;
    HUDElementColors colors;
    uint16_t frame_rate;
    PAD_CHARS(2);
    uint16_t sequence_index;
    struct {
        uint16_t show_on_flashing : 1;
        uint16_t show_on_empty : 1;
        uint16_t show_on_reload_overheating : 1;
        uint16_t show_on_default : 1;
        uint16_t show_always : 1;
        PAD_BITS(uint16_t, 11);
    } type;
    uint8_t flashes_when_active;
    PAD_CHARS(3);
    PAD_CHARS(0x38);
};

static_assert(sizeof(HUDOverlay) == 0x88);

enum HUDMultitextureOverlayAnchor : uint16_t {
    MULTITEXTURE_OVERLAY_ANCHOR_TEXTURE = 0,
    MULTITEXTURE_OVERLAY_ANCHOR_SCREEN
};

enum HUDMultitextureBlendingFunction : uint16_t {
    MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_ADD = 0,
    MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_SUBTRACT,
    MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_MULTIPLY,
    MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_MULTIPLY_2X,
    MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_DOT
};

enum HUDMultitextureWrapMode : uint16_t {
    MULTITEXTURE_OVERLAY_WRAP_CLAMP = 0,
    MULTITEXTURE_OVERLAY_WRAP_WRAP
};

struct HUDMultitextureOverlay {
    PAD_CHARS(2);
    uint16_t type;
    enum : uint16_t {
        FRAMEBUFFER_BLEND_ALPHA_BLEND = 0,
        FRAMEBUFFER_BLEND_MULTIPLY,
        FRAMEBUFFER_BLEND_DOUBLE_MULTIPLY,
        FRAMEBUFFER_BLEND_ADD,
        FRAMEBUFFER_BLEND_SUBTRACT,
        FRAMEBUFFER_BLEND_COMPONENT_MINIMUM,
        FRAMEBUFFER_BLEND_COMPONENT_MAXIMUM,
        FRAMEBUFFER_BLEND_ALPHA_MULTIPLY_ADD
    } blend_function;
    PAD_CHARS(0x22);
    HUDMultitextureOverlayAnchor primary_anchor;
    HUDMultitextureOverlayAnchor secondary_anchor;
    HUDMultitextureOverlayAnchor teritary_anchor;
    HUDMultitextureBlendingFunction blending_function_0_to_1;
    HUDMultitextureBlendingFunction blending_function_1_to_2;
    PAD_CHARS(2);

    struct {
        float primary_scale_x;
        float primary_scale_y;
        float secondary_scale_x;
        float secondary_scale_y;
        float tertiary_scale_x;
        float tertiary_scale_y;
    } map_scales;

    struct {
        float primary_offset_x;
        float primary_offset_y;
        float secondary_offset_x;
        float secondary_offset_y;
        float tertiary_offset_x;
        float tertiary_offset_y;
    } map_offsets;

    struct {
        HaloTagDependency primary;
        HaloTagDependency secondary;
        HaloTagDependency tertiary;

        HUDMultitextureWrapMode primary_wrap_mode;
        HUDMultitextureWrapMode seconday_wrap_mode;
        HUDMultitextureWrapMode tertiary_wrap_mode;

        PAD_CHARS(2);
    } map;

    PAD_CHARS(0xB8);

    uint32_t effectors_count;
    void *effectors;
    PAD_CHARS(4);

    PAD_CHARS(0x80);
};

static_assert(sizeof(HUDMultitextureOverlay) == 0x1E0);

struct GrenadeHUDInterfaceS {
    HUDElementPosition position;
    HaloTagDependency bitmap;

    HUDElementColors colors;
    uint16_t sequence_index;
    uint32_t multitexture_overlays_count;
    HUDMultitextureOverlay *multitexture_overlays;
    PAD_CHARS(4);
    PAD_CHARS(4);
};

struct GrenadeHUDInterfaceWarningSoundLatchedTo {
    uint32_t low_grenade_count : 1;
    uint32_t no_grenades_left : 1;
    uint32_t throw_on_no_grenades : 1;
    PAD_BITS(uint32_t, 29);
};

struct GrenadeHUDInterface {
    HUDAnchor anchor;
    PAD_CHARS(0x22);
    GrenadeHUDInterfaceS grenade_hud_background;
    GrenadeHUDInterfaceS total_grenades_background;
    struct {
        HUDElementPosition position;
        HUDElementColors colors;
        uint8_t maximum_number_of_digits;
        HUDNumberElementFlags flags;
        uint8_t number_of_fractional_digits;
        PAD_CHARS(0xD);
        uint16_t flash_cutoff;
        PAD_CHARS(2);
        HaloTagDependency overlay_bitmap;
    } total_grenades_numbers;
    uint32_t overlays_count;
    HUDOverlay *overlays;
    PAD_CHARS(4);
    uint32_t warning_sounds_count;
    HUDInterfaceWarningSound<GrenadeHUDInterfaceWarningSoundLatchedTo> *warning_sounds;
    PAD_CHARS(4);
    PAD_CHARS(0x44);
    struct {
        uint16_t sequence_index;
        uint16_t width_offset;
        AnchorOffset offset_from_reference_corner;
        ColorByte override_icon_color;
        uint8_t frame_rate;
        struct {
            uint8_t use_text_from_string_list_instead : 1;
            uint8_t override_default_color : 1;
            uint8_t width_Offset_is_absolute_icon_width : 1;
            PAD_BITS(uint8_t, 5);
        } flags;
        uint16_t text_index;
    } messaging_information;
};

static_assert(sizeof(GrenadeHUDInterface) == 0x1C8);
static_assert(sizeof(HUDInterfaceWarningSound<GrenadeHUDInterfaceWarningSoundLatchedTo>) == 0x38);

enum WeaponHUDInterfaceStateAttachedTo : uint16_t {
    STATE_ATTACHED_TO_TOTAL_AMMO = 0,
    STATE_ATTACHED_TO_LOADED_AMMO,
    STATE_ATTACHED_TO_HEAT,
    STATE_ATTACHED_TO_AGE,
    STATE_ATTACHED_TO_SECONDARY_WEAPON_TOTAL_AMMO,
    STATE_ATTACHED_TO_SECONDARY_WEAPON_LOADED_AMMO,
    STATE_ATTACHED_TO_DISTANCE_TO_TARGET,
    STATE_ATTACHED_TO_ELEVATION_TO_TARGET
};

struct WeaponHUDInterfaceElementPrelude {
    WeaponHUDInterfaceStateAttachedTo state_attached_to;

    PAD_CHARS(2);
    HUDMapType can_use_on_map_type;
    PAD_CHARS(0x1E);

    HUDElementPosition position;
};

static_assert(sizeof(WeaponHUDInterfaceElementPrelude) == 0x48);

struct WeaponHUDInterfaceStaticElement : WeaponHUDInterfaceElementPrelude {
    HaloTagDependency interface_bitmap;
    HUDElementColors colors;

    uint16_t sequence_index;
    uint32_t multitexture_overlay_count;
    HUDMultitextureOverlay *multitexture_overlays;
    PAD_CHARS(0x4);
    PAD_CHARS(0x2C);
};

static_assert(sizeof(WeaponHUDInterfaceStaticElement) == 0xB4);

struct WeaponHUDInterfaceMeterElement : WeaponHUDInterfaceElementPrelude {
    HaloTagDependency meter_bitmap;
    HUDMeterColors colors;
    PAD_CHARS(0x38);
};

static_assert(sizeof(WeaponHUDInterfaceMeterElement) == 0xB4);

struct WeaponHUDInterfaceNumberElement : WeaponHUDInterfaceElementPrelude {
    HUDElementColors colors;
    int8_t maximum_number_of_digits;
    HUDNumberElementFlags flags;
    uint16_t number_of_fractional_digits;
    PAD_CHARS(0xC);
    uint8_t divide_number_by_clip_size;
    PAD_CHARS(0x27);
};

static_assert(sizeof(WeaponHUDInterfaceNumberElement) == 0xA0);

struct WeaponHUDInterfaceCrosshairOverlay {
    HUDElementPosition position;
    HUDElementColors colors;
    uint16_t frame_rate;
    uint16_t sequence_index;
    struct {
        uint32_t flashes_when_active : 1;
        uint32_t not_a_sprite : 1;
        uint32_t shows_only_when_zoomed : 1;
        uint32_t show_sniper_data : 1;
        uint32_t hide_area_outside_reticle : 1;
        uint32_t one_zoom_level : 1;
        uint32_t dont_show_when_zoomed : 1;
        PAD_BITS(uint32_t, 25);
    } flags;

    PAD_CHARS(0x20);
};

static_assert(sizeof(WeaponHUDInterfaceCrosshairOverlay) == 0x6C);

struct WeaponHUDInterfaceOverlayElement {
    WeaponHUDInterfaceStateAttachedTo state_attached_to;
    PAD_CHARS(2);
    HUDMapType can_use_on_map_type;
    PAD_CHARS(2);
    PAD_CHARS(0x1C);
    HaloTagDependency overlay_bitmap;
    uint32_t overlays_count;
    HUDOverlay *overlays;
    PAD_CHARS(4);
    PAD_CHARS(0x28);
};

static_assert(sizeof(WeaponHUDInterfaceOverlayElement) == 0x68);

struct WeaponHUDInterfaceCrosshair {
    enum : uint16_t {
        CROSSHAIR_AIM = 0,
        CROSSHAIR_ZOOM,
        CROSSHAIR_CHARGE,
        CROSSHAIR_SHOULD_RELOAD,
        CROSSHAIR_FLASH_HEAT,
        CROSSHAIR_FLASH_TOTAL_AMMO,
        CROSSHAIR_FLASH_BATTERY,
        CROSSHAIR_RELOAD_OVERHEAT,
        CROSSHAIR_FLASH_WHEN_FIRING_AND_NO_AMMO,
        CROSSHAIR_FLASH_WHEN_THROWING_AND_NO_GRENADE,
        CROSSHAIR_LOW_AMMO_AND_NONE_LEFT_TO_RELOAD,
        CROSSHAIR_SHOULD_RELOAD_SECONDARY_TRIGGER,
        CROSSHAIR_FLASH_SECONDARY_TOTAL_AMMO,
        CROSSHAIR_FLASH_SECONDARY_RELOAD,
        CROSSHAIR_FLASH_WHEN_FIRING_SECONDARY_TRIGGER_WITH_NO_AMMO,
        CROSSHAIR_LOW_SECONDARY_AMMO_AND_NONE_LEFT_TO_RELOAD,
        CROSSHAIR_PRIMARY_TRIGGER_READY,
        CROSSHAIR_SECONDARY_TRIGGER_READY,
        CROSSHAIR_FLASH_WHEN_FIRING_WITH_DEPLETED_BATTERY
    } crosshair_type;
    PAD_CHARS(2);
    HUDMapType can_use_on_map_type;
    PAD_CHARS(2);
    PAD_CHARS(0x1C);
    HaloTagDependency crosshair_bitmap;
    uint32_t crosshair_overlays_count;
    WeaponHUDInterfaceCrosshairOverlay *crosshair_overlays;
    PAD_CHARS(0x4);
    PAD_CHARS(0x28);
};

static_assert(sizeof(WeaponHUDInterfaceCrosshair) == 0x68);

struct WeaponHUDInterface {
    HaloTagDependency child;
    uint8_t use_parent_hud_flashing_parameters : 1;
    PAD_BITS(uint8_t, 7);
    PAD_CHARS(0x3);

    struct {
        int16_t total_ammo_cutoff;
        int16_t loaded_ammo_cutoff;
        int16_t heat_cutoff;
        int16_t age_cutoff;
    } flash_cutoffs;

    PAD_CHARS(0x20);
    HUDAnchor anchor;
    PAD_CHARS(0x22);

    uint32_t static_elements_count;
    WeaponHUDInterfaceStaticElement *static_elements;
    PAD_CHARS(0x4);

    uint32_t meter_elements_count;
    WeaponHUDInterfaceMeterElement *meter_elements;
    PAD_CHARS(0x4);

    uint32_t number_elements_count;
    WeaponHUDInterfaceNumberElement *number_elements;
    PAD_CHARS(0x4);

    uint32_t crosshairs_count;
    WeaponHUDInterfaceCrosshair *crosshairs;
    PAD_CHARS(0x4);

    uint32_t overlay_elements_count;
    WeaponHUDInterfaceOverlayElement *overlay_elements;
    PAD_CHARS(0x4);

    PAD_CHARS(0x10);

    uint32_t screen_effects_count;
    void *screen_effects;
    PAD_CHARS(0x4);
};

static_assert(sizeof(WeaponHUDInterface) == 0xB8);

struct UnitHUDInterfaceBackground {
    HUDElementPosition position;
    HaloTagDependency bitmap;
    HUDElementColors colors;
    uint16_t sequence_index;
    PAD_CHARS(0x2);
    uint32_t multitexture_overlay_count;
    HUDMultitextureOverlay *multitexture_overlays;
    PAD_CHARS(4);
    PAD_CHARS(4);
};

static_assert(sizeof(UnitHUDInterfaceBackground) == 0x68);

struct UnitHUDInterfaceMeterPrelude {
    HUDElementPosition position;
    HaloTagDependency bitmap;
    HUDMeterColors colors;
    PAD_CHARS(0x10);
};

static_assert(sizeof(UnitHUDInterfaceMeterPrelude) == 0x68);

struct UnitHUDInterfaceWarningSoundLatchedTo {
    uint32_t shield_recharging : 1;
    uint32_t shield_damaged : 1;
    uint32_t shield_low : 1;
    uint32_t shield_empty : 1;
    uint32_t health_low : 1;
    uint32_t health_empty : 1;
    uint32_t health_minor_damage : 1;
    uint32_t health_major_damage : 1;
    PAD_BITS(uint32_t, 24);
};

struct UnitHUDInterfaceAuxiliaryHUDMeter {
    PAD_CHARS(0x14);
    UnitHUDInterfaceBackground background;
    struct : UnitHUDInterfaceMeterPrelude {
        float minimum_fraction_cutoff;
        struct {
            uint16_t show_only_when_active : 1;
            uint16_t flash_once_if_activated_while_disabled : 1;
            PAD_BITS(uint16_t, 14);
        };
        PAD_CHARS(2);
    } meter;
    PAD_CHARS(0x58);
};

static_assert(sizeof(UnitHUDInterfaceAuxiliaryHUDMeter) == 0x144);

struct UnitHUDInterface {
    HUDAnchor anchor;
    PAD_CHARS(0x22);
    UnitHUDInterfaceBackground unit_hud_background;
    UnitHUDInterfaceBackground shield_panel_background;
    struct : UnitHUDInterfaceMeterPrelude {
        ColorByte overcharge_minimum_color;
        ColorByte overcharge_maximum_color;
        ColorByte overcharge_flash_color;
        ColorByte overcharge_empty_color;
        PAD_CHARS(0x10);
    } shield_panel_meter;
    UnitHUDInterfaceBackground health_panel_background;
    struct : UnitHUDInterfaceMeterPrelude {
        ColorByte medium_health_left_color;
        float maximum_color_health_fraction_cutoff;
        float minumum_color_health_fraction_cutoff;
        PAD_CHARS(0x14);
    } health_panel_meter;
    UnitHUDInterfaceBackground motion_sensor_background;
    UnitHUDInterfaceBackground motion_sensor_foreground;
    PAD_CHARS(0x20);
    HUDElementPosition motion_sensor_center;
    struct {
        HUDAnchor anchor;
        PAD_CHARS(0x22);
        uint32_t overlays_count;
        HUDOverlay *overlays;
        PAD_CHARS(4);
    } auxiliary_overlays;
    PAD_CHARS(0x10);
    uint32_t warning_sounds_count;
    HUDInterfaceWarningSound<UnitHUDInterfaceWarningSoundLatchedTo> *warning_sounds;
    PAD_CHARS(4);
    uint32_t auxiliary_hud_meters_count;
    UnitHUDInterfaceAuxiliaryHUDMeter *auxiliary_hud_meters;
    PAD_CHARS(4);
};

static_assert(sizeof(UnitHUDInterface) == 0x3D8);
