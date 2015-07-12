Description
this is a UE4 game with a C++ character extension
Character's beginPlay spawns a thread that non-blockingly polls for UDP input
Blueprint polls the thread for input
Blueprint adds the difference between current and last inputs to the transform of the ghost sword
Blueprint uses physics to make the real, game interactable sword aproach the ghost sword

Warning
I can't seem to get the icon to not exclude and it's required to be in Build/Windows/ for the project to compile. I stuck it in the top level so move it before building.

TODO
add time since polled variable to scale input in blueprint based off when it was last accessed
add a practise dummy to hit. Most simple is a second ghost-real sword pair with an AI controlled ghost sword hovering nearby

Notes to self
Android orientation is in radians
0 => -pi, pi => 0 
except pitch which goes 0 => -pi/2 => 0 => pi/2 => 0 
whether pitch is negative or positive depends on whether yaw is positive or negative. Yaw changes when pitch goes through pi/2
Android XYZ is yaw pitch roll while UE4 assumes pitch yaw roll

rules
// potential lies; if pitch positive and roll negative then shift up by 180

/*
switch (mScreenRotation) {
    case Surface.ROTATION_0:
        axisX = SensorManager.AXIS_X;
    axisY = SensorManager.AXIS_Y;
        break;

    case Surface.ROTATION_90:
        axisX = SensorManager.AXIS_Y;
    axisY = SensorManager.AXIS_MINUS_X;
        break;

    case Surface.ROTATION_180:
        axisX = SensorManager.AXIS_MINUS_X;
    axisY = SensorManager.AXIS_MINUS_Y;
        break;

    case Surface.ROTATION_270:
        axisX = SensorManager.AXIS_MINUS_Y;
    axisY = SensorManager.AXIS_X;
        break;

    default:
        break;
}
*/