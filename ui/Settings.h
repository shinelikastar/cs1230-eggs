/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "lib/RGBA.h"

/**
 * @struct Settings
 *
 * Stores application settings for the CS123 GUI.
 *
 * You can access all app settings through the "settings" global variable.
 * The settings will be automatically updated when things are changed in the
 * GUI (the reverse is not true however: changing the value of a setting does
 * not update the GUI).
*/
struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    // Bézier camera
    bool paused;
    int facingEgg;
    float height;
    float scale;
    int pathNum;

    // Depth of field
    bool enableDof;
    float fStop;
    float planeInFocus;

    // Mapping
    bool enableTextures;
    bool enableDisplacement;

    // Camera
    float cameraFov;
    float cameraNear;
    float cameraFar;

    // Lighting
    bool usePointLights;
    bool useDirectionalLights;
    bool useLighting;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
