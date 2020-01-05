/*!

 Settings.h
 CS123 Support Code

 @author  Evan Wallace (edwallac)
 @date    9/1/2010

 This file contains various settings and enumerations that you will need to
 use in the various assignments. The settings are bound to the GUI via static
 data bindings.

**/

#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
 * Loads the application settings, or, if no saved settings are available, loads default values for
 * the settings. You can change the defaults here.
 */
void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    QSettings s("Final", "Final");

    // Bézier
    paused =  s.value("paused", false).toBool();
    facingEgg =  s.value("facingEgg", false).toBool();
    height = s.value("height", 2.5f).toDouble();
    scale = s.value("scale", 2.5f).toDouble();
    pathNum = s.value("pathNum", 1).toInt();

    // Mapping
    enableTextures = s.value("enableTextures", true).toBool();
    enableDisplacement = s.value("enableDisplacement", true).toBool();

    // Depth of Field
    enableDof = s.value("enableDof", false).toBool();
    fStop = s.value("fStop", 1.4f).toDouble();
    planeInFocus = s.value("planeInFocus", 10.f).toDouble();

    // Camera
    cameraFov = s.value("cameraFov", 50.).toDouble();
    cameraNear = s.value("cameraNear", 0.1).toDouble();
    cameraFar = s.value("cameraFar", 50).toDouble();

    // Lighting
    useLighting = s.value("useLighting", true).toBool();
    usePointLights = s.value("usePointLights", true).toBool();
    useDirectionalLights = s.value("useDirectionalLights", true).toBool();

}

void Settings::saveSettings() {
    QSettings s("Final", "Final");

    // Bézier
    s.setValue("paused", paused);
    s.setValue("height", height);
    s.setValue("scale", scale);
    s.setValue("pathNum", pathNum);

    // Mapping
    s.setValue("enableTextures", enableTextures);
    s.setValue("enableDisplacement", enableDisplacement);

    // Depth of Field
    s.setValue("enableDof", enableDof);
    s.setValue("fStop", fStop);
    s.setValue("planeInFocus", planeInFocus);

    // Camera
    s.setValue("cameraFov", cameraFov);
    s.setValue("cameraNear", cameraNear);
    s.setValue("cameraFar", cameraFar);

    // Lighting
    s.setValue("useLighting", useLighting);
    s.setValue("usePointLights", usePointLights);
    s.setValue("useDirectionalLights", useDirectionalLights);
}
