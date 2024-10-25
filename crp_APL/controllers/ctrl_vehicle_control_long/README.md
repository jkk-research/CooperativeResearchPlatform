# Ctrl Vehicle Control

A package for trajectory following, Lateral and Longitudinal Control of the Vehicle in the APL layer. 

## APL level Controller of the Vehicle

The Controller uses a PID controller, with a Feedback and FeedForward layer, the error in the Feedback loop is based on lateral and angle errror. 

### Usage:

```
 ros2 launch ctrl_vehicle_control ctrl_vehicle_control.launch.py
```

### Parameters
| Name             | Type   | Default value                                   |
|------------------|--------|-------------------------------------------------|
| /ctrl/ffGainOffsetGround    | double | 1.0                                             |
| /ctrl/ffGainSlope | double | 0.1 |
| /ctrl/ffLookAheadTime | double | 1.5
| /ctrl/steeringAngleLPFilter | double | 0.7
| /ctrl/fbLookAheadTime | double | 0.5
| /ctrl/fbPGain | double | 0.01
| /ctrl/fbDGain | double | 0.0
| /ctrl/fbIGain | double | 0.0
| /ctrl/fbMinLookAheadDistance | double | 0.0
| /ctrl/fbIntegralLimit | double | 3.0  