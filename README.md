# InvertedPendulum
This Robocomp component implements the control system of an inverted pendulum.


## Config parameters
As any other component, *InvertedPendulum* needs a configuration file to start. In
```
etc/config
```
In this case, we can find there the following lines:
```
CommonBehavior.Endpoints=tcp -p 10000

# Proxies for required interfaces
IMUProxy = imu:tcp -h localhost -p 10005
JointMotorSimpleProxy = jointmotorsimple:tcp -h localhost -p 10006



InnerModelPath = innermodel.xml

Ice.Warn.Connections=0
Ice.Trace.Network=0
Ice.Trace.Protocol=0
Ice.MessageSizeMax=20004800
```

## Starting the component
To start the component, you'll just have to execute the following line:

```
bin/InvertedPendulum etc/config
```
