#include "hce_tag_class_int.h"
#include <string.h>

using namespace HaloCE;

const char *HaloCE::tag_class_string_from_int(TagClassInt i) noexcept {
    switch (i)
    {
    case TAG_CLASS_INT_ACTOR: return "actor";
    case TAG_CLASS_INT_ACTOR_VARIANT: return "actor_variant";
    case TAG_CLASS_INT_ANTENNA: return "antenna";
    case TAG_CLASS_INT_MODEL_ANIMATIONS: return "model_animations";
    case TAG_CLASS_INT_BIPED: return "biped";
    case TAG_CLASS_INT_BITMAP: return "bitmap";
    case TAG_CLASS_INT_SPHEROID: return "spheroid";
    case TAG_CLASS_INT_CONTINUOUS_DAMAGE_EFFECT: return "continuous_damage_effect";
    case TAG_CLASS_INT_MODEL_COLLISION_GEOMETRY: return "model_collision_geometry";
    case TAG_CLASS_INT_COLOR_TABLE: return "color_table";
    case TAG_CLASS_INT_CONTRAIL: return "contrail";
    case TAG_CLASS_INT_DEVICE_CONTROL: return "device_control";
    case TAG_CLASS_INT_DECAL: return "decal";
    case TAG_CLASS_INT_UI_WIDGET_DEFINITION: return "ui_widget_definition";
    case TAG_CLASS_INT_INPUT_DEVICE_DEFAULTS: return "input_device_defaults";
    case TAG_CLASS_INT_DEVICE: return "device";
    case TAG_CLASS_INT_DETAIL_OBJECT_COLLECTION: return "detail_object_collection";
    case TAG_CLASS_INT_EFFECT: return "effect";
    case TAG_CLASS_INT_EQUIPMENT: return "equipment";
    case TAG_CLASS_INT_FLAG: return "flag";
    case TAG_CLASS_INT_FOG: return "fog";
    case TAG_CLASS_INT_FONT: return "font";
    case TAG_CLASS_INT_LIGHTNING: return "lightning";
    case TAG_CLASS_INT_MATERIAL_EFFECTS: return "material_effects";
    case TAG_CLASS_INT_GARBAGE: return "garbage";
    case TAG_CLASS_INT_GLOW: return "glow";
    case TAG_CLASS_INT_GRENADE_HUD_INTERFACE: return "grenade_hud_interface";
    case TAG_CLASS_INT_HUD_MESSAGE_TEXT: return "hud_message_text";
    case TAG_CLASS_INT_HUD_NUMBER: return "hud_number";
    case TAG_CLASS_INT_HUD_GLOBALS: return "hud_globals";
    case TAG_CLASS_INT_ITEM: return "item";
    case TAG_CLASS_INT_ITEM_COLLECTION: return "item_collection";
    case TAG_CLASS_INT_DAMAGE_EFFECT: return "damage_effect";
    case TAG_CLASS_INT_LENS_FLARE: return "lens_flare";
    case TAG_CLASS_INT_DEVICE_LIGHT_FIXTURE: return "device_light_fixture";
    case TAG_CLASS_INT_LIGHT: return "light";
    case TAG_CLASS_INT_SOUND_LOOPING: return "sound_looping";
    case TAG_CLASS_INT_DEVICE_MACHINE: return "device_machine";
    case TAG_CLASS_INT_GLOBALS: return "globals";
    case TAG_CLASS_INT_METER: return "meter";
    case TAG_CLASS_INT_LIGHT_VOLUME: return "light_volume";
    case TAG_CLASS_INT_GBXMODEL: return "gbxmodel";
    case TAG_CLASS_INT_MODEL: return "model";
    case TAG_CLASS_INT_MULTIPLAYER_SCENARIO_DESCRIPTION: return "multiplayer_scenario_description";
    case TAG_CLASS_INT_PREFERENCES_NETWORK_GAME: return "preferences_network_game";
    case TAG_CLASS_INT_OBJECT: return "object";
    case TAG_CLASS_INT_PARTICLE: return "particle";
    case TAG_CLASS_INT_PARTICLE_SYSTEM: return "particle_system";
    case TAG_CLASS_INT_PHYSICS: return "physics";
    case TAG_CLASS_INT_PLACEHOLDER: return "placeholder";
    case TAG_CLASS_INT_POINT_PHYSICS: return "point_physics";
    case TAG_CLASS_INT_PROJECTILE: return "projectile";
    case TAG_CLASS_INT_WEATHER: return "weather";
    case TAG_CLASS_INT_SCENARIO_STRUCTURE_BSP: return "scenario_structure_bsp";
    case TAG_CLASS_INT_SCENERY: return "scenery";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_CHICAGO_EXTENDED: return "shader_transparent_chicago_extended";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_CHICAGO: return "shader_transparent_chicago";
    case TAG_CLASS_INT_SCENARIO: return "scenario";
    case TAG_CLASS_INT_SHADER_ENVIRONMENT: return "shader_environment";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_GLASS: return "transparent_glass";
    case TAG_CLASS_INT_SHADER: return "shader";
    case TAG_CLASS_INT_SKY: return "sky";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_METER: return "shader_transparent_meter";
    case TAG_CLASS_INT_SOUND: return "sound";
    case TAG_CLASS_INT_SOUND_ENVIRONMENT: return "sound_environment";
    case TAG_CLASS_INT_SHADER_MODEL: return "shader_model";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_GENERIC: return "shader_transparent_generic";
    case TAG_CLASS_INT_UI_WIDGET_COLLECTION: return "ui_widget_collection";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_PLASMA: return "shader_transparent_plasma";
    case TAG_CLASS_INT_SOUND_SCENERY: return "sound_scenery";
    case TAG_CLASS_INT_STRING_LIST: return "string_list";
    case TAG_CLASS_INT_SHADER_TRANSPARENT_WATER: return "shader_transparent_water";
    case TAG_CLASS_INT_TAG_COLLECTION: return "tag_collection";
    case TAG_CLASS_INT_CAMERA_TRACK: return "camera_track";
    case TAG_CLASS_INT_UNIT_DIALOGUE: return "unit_dialogue";
    case TAG_CLASS_INT_UNIT_HUD_INTERFACE: return "unit_hud_interface";
    case TAG_CLASS_INT_UNIT: return "unit";
    case TAG_CLASS_INT_UNICODE_STRING_LIST: return "unicode_string_list";
    case TAG_CLASS_INT_VIRTUAL_KEYBOARD: return "virtual_keyboard";
    case TAG_CLASS_INT_VEHICLE: return "vehicle";
    case TAG_CLASS_INT_WEAPON: return "weapon";
    case TAG_CLASS_INT_WIND: return "wind";
    case TAG_CLASS_INT_WEAPON_HUD_INTERFACE: return "weapon_hud_interface";
    case TAG_CLASS_INT_NONE: return "none";
    default: return "unknown";
    }
}

TagClassInt HaloCE::tag_class_int_from_string(const char *i) noexcept {
    if (strcmp(i, "actor") == 0)
        return TAG_CLASS_INT_ACTOR;
    else if (strcmp(i, "actor_variant") == 0)
        return TAG_CLASS_INT_ACTOR_VARIANT;
    else if (strcmp(i, "antenna") == 0)
        return TAG_CLASS_INT_ANTENNA;
    else if (strcmp(i, "model_animations") == 0)
        return TAG_CLASS_INT_MODEL_ANIMATIONS;
    else if (strcmp(i, "biped") == 0)
        return TAG_CLASS_INT_BIPED;
    else if (strcmp(i, "bitmap") == 0)
        return TAG_CLASS_INT_BITMAP;
    else if (strcmp(i, "spheroid") == 0)
        return TAG_CLASS_INT_SPHEROID;
    else if (strcmp(i, "continuous_damage_effect") == 0)
        return TAG_CLASS_INT_CONTINUOUS_DAMAGE_EFFECT;
    else if (strcmp(i, "model_collision_geometry") == 0)
        return TAG_CLASS_INT_MODEL_COLLISION_GEOMETRY;
    else if (strcmp(i, "color_table") == 0)
        return TAG_CLASS_INT_COLOR_TABLE;
    else if (strcmp(i, "contrail") == 0)
        return TAG_CLASS_INT_CONTRAIL;
    else if (strcmp(i, "device_control") == 0)
        return TAG_CLASS_INT_DEVICE_CONTROL;
    else if (strcmp(i, "decal") == 0)
        return TAG_CLASS_INT_DECAL;
    else if (strcmp(i, "ui_widget_definition") == 0)
        return TAG_CLASS_INT_UI_WIDGET_DEFINITION;
    else if (strcmp(i, "input_device_defaults") == 0)
        return TAG_CLASS_INT_INPUT_DEVICE_DEFAULTS;
    else if (strcmp(i, "device") == 0)
        return TAG_CLASS_INT_DEVICE;
    else if (strcmp(i, "detail_object_collection") == 0)
        return TAG_CLASS_INT_DETAIL_OBJECT_COLLECTION;
    else if (strcmp(i, "effect") == 0)
        return TAG_CLASS_INT_EFFECT;
    else if (strcmp(i, "equipment") == 0)
        return TAG_CLASS_INT_EQUIPMENT;
    else if (strcmp(i, "flag") == 0)
        return TAG_CLASS_INT_FLAG;
    else if (strcmp(i, "fog") == 0)
        return TAG_CLASS_INT_FOG;
    else if (strcmp(i, "font") == 0)
        return TAG_CLASS_INT_FONT;
    else if (strcmp(i, "lightning") == 0)
        return TAG_CLASS_INT_LIGHTNING;
    else if (strcmp(i, "material_effects") == 0)
        return TAG_CLASS_INT_MATERIAL_EFFECTS;
    else if (strcmp(i, "garbage") == 0)
        return TAG_CLASS_INT_GARBAGE;
    else if (strcmp(i, "glow") == 0)
        return TAG_CLASS_INT_GLOW;
    else if (strcmp(i, "grenade_hud_interface") == 0)
        return TAG_CLASS_INT_GRENADE_HUD_INTERFACE;
    else if (strcmp(i, "hud_message_text") == 0)
        return TAG_CLASS_INT_HUD_MESSAGE_TEXT;
    else if (strcmp(i, "hud_number") == 0)
        return TAG_CLASS_INT_HUD_NUMBER;
    else if (strcmp(i, "hud_globals") == 0)
        return TAG_CLASS_INT_HUD_GLOBALS;
    else if (strcmp(i, "item") == 0)
        return TAG_CLASS_INT_ITEM;
    else if (strcmp(i, "item_collection") == 0)
        return TAG_CLASS_INT_ITEM_COLLECTION;
    else if (strcmp(i, "damage_effect") == 0)
        return TAG_CLASS_INT_DAMAGE_EFFECT;
    else if (strcmp(i, "lens_flare") == 0)
        return TAG_CLASS_INT_LENS_FLARE;
    else if (strcmp(i, "device_light_fixture") == 0)
        return TAG_CLASS_INT_DEVICE_LIGHT_FIXTURE;
    else if (strcmp(i, "light") == 0)
        return TAG_CLASS_INT_LIGHT;
    else if (strcmp(i, "sound_looping") == 0)
        return TAG_CLASS_INT_SOUND_LOOPING;
    else if (strcmp(i, "device_machine") == 0)
        return TAG_CLASS_INT_DEVICE_MACHINE;
    else if (strcmp(i, "globals") == 0)
        return TAG_CLASS_INT_GLOBALS;
    else if (strcmp(i, "meter") == 0)
        return TAG_CLASS_INT_METER;
    else if (strcmp(i, "light_volume") == 0)
        return TAG_CLASS_INT_LIGHT_VOLUME;
    else if (strcmp(i, "gbxmodel") == 0)
        return TAG_CLASS_INT_GBXMODEL;
    else if (strcmp(i, "model") == 0)
        return TAG_CLASS_INT_MODEL;
    else if (strcmp(i, "multiplayer_scenario_description") == 0)
        return TAG_CLASS_INT_MULTIPLAYER_SCENARIO_DESCRIPTION;
    else if (strcmp(i, "preferences_network_game") == 0)
        return TAG_CLASS_INT_PREFERENCES_NETWORK_GAME;
    else if (strcmp(i, "object") == 0)
        return TAG_CLASS_INT_OBJECT;
    else if (strcmp(i, "particle") == 0)
        return TAG_CLASS_INT_PARTICLE;
    else if (strcmp(i, "particle_system") == 0)
        return TAG_CLASS_INT_PARTICLE_SYSTEM;
    else if (strcmp(i, "physics") == 0)
        return TAG_CLASS_INT_PHYSICS;
    else if (strcmp(i, "placeholder") == 0)
        return TAG_CLASS_INT_PLACEHOLDER;
    else if (strcmp(i, "point_physics") == 0)
        return TAG_CLASS_INT_POINT_PHYSICS;
    else if (strcmp(i, "projectile") == 0)
        return TAG_CLASS_INT_PROJECTILE;
    else if (strcmp(i, "weather") == 0)
        return TAG_CLASS_INT_WEATHER;
    else if (strcmp(i, "scenario_structure_bsp") == 0)
        return TAG_CLASS_INT_SCENARIO_STRUCTURE_BSP;
    else if (strcmp(i, "scenery") == 0)
        return TAG_CLASS_INT_SCENERY;
    else if (strcmp(i, "shader_transparent_chicago_extended") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_CHICAGO_EXTENDED;
    else if (strcmp(i, "shader_transparent_chicago") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_CHICAGO;
    else if (strcmp(i, "scenario") == 0)
        return TAG_CLASS_INT_SCENARIO;
    else if (strcmp(i, "shader_environment") == 0)
        return TAG_CLASS_INT_SHADER_ENVIRONMENT;
    else if (strcmp(i, "transparent_glass") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_GLASS;
    else if (strcmp(i, "shader") == 0)
        return TAG_CLASS_INT_SHADER;
    else if (strcmp(i, "sky") == 0)
        return TAG_CLASS_INT_SKY;
    else if (strcmp(i, "shader_transparent_meter") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_METER;
    else if (strcmp(i, "sound") == 0)
        return TAG_CLASS_INT_SOUND;
    else if (strcmp(i, "sound_environment") == 0)
        return TAG_CLASS_INT_SOUND_ENVIRONMENT;
    else if (strcmp(i, "shader_model") == 0)
        return TAG_CLASS_INT_SHADER_MODEL;
    else if (strcmp(i, "shader_transparent_generic") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_GENERIC;
    else if (strcmp(i, "ui_widget_collection") == 0)
        return TAG_CLASS_INT_UI_WIDGET_COLLECTION;
    else if (strcmp(i, "shader_transparent_plasma") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_PLASMA;
    else if (strcmp(i, "sound_scenery") == 0)
        return TAG_CLASS_INT_SOUND_SCENERY;
    else if (strcmp(i, "string_list") == 0)
        return TAG_CLASS_INT_STRING_LIST;
    else if (strcmp(i, "shader_transparent_water") == 0)
        return TAG_CLASS_INT_SHADER_TRANSPARENT_WATER;
    else if (strcmp(i, "tag_collection") == 0)
        return TAG_CLASS_INT_TAG_COLLECTION;
    else if (strcmp(i, "camera_track") == 0)
        return TAG_CLASS_INT_CAMERA_TRACK;
    else if (strcmp(i, "unit_dialogue") == 0)
        return TAG_CLASS_INT_UNIT_DIALOGUE;
    else if (strcmp(i, "unit_hud_interface") == 0)
        return TAG_CLASS_INT_UNIT_HUD_INTERFACE;
    else if (strcmp(i, "unit") == 0)
        return TAG_CLASS_INT_UNIT;
    else if (strcmp(i, "unicode_string_list") == 0)
        return TAG_CLASS_INT_UNICODE_STRING_LIST;
    else if (strcmp(i, "virtual_keyboard") == 0)
        return TAG_CLASS_INT_VIRTUAL_KEYBOARD;
    else if (strcmp(i, "vehicle") == 0)
        return TAG_CLASS_INT_VEHICLE;
    else if (strcmp(i, "weapon") == 0)
        return TAG_CLASS_INT_WEAPON;
    else if (strcmp(i, "wind") == 0)
        return TAG_CLASS_INT_WIND;
    else if (strcmp(i, "weapon_hud_interface") == 0)
        return TAG_CLASS_INT_WEAPON_HUD_INTERFACE;
    else 
        return TAG_CLASS_INT_NONE;
}
