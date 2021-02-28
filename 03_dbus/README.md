# Dbus learn by example

First try to understand the [example](https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gio/ch30s05.html#gdbus-example-codegen-server) by generating the server side code and get idea how dbus server looks like. It's important to get to know those d-bus words.

Then try to check the cliet code and refer to GIO reference manual to understand used module like GDBusProxy.

## Server code

[Jump to example](./01_server/README.md).

## Client code

[Client code using generated code](./02_client_generated_code) shows client code using the generated code from example.xml.


[Client code using proxy](./03_client_proxy/README.md) not from generated code but using GDBusProxy class.

## Debug

Besides `dfeet`, `dbus-monitor` provides more real time monitoring of dbus events.

If we want to listen to signal from "com.test.animal", we can use `dbus-monitor "type=signal,sender='com.test.animal'"` as an example.

Check [match rule](https://dbus.freedesktop.org/doc/dbus-specification.html#message-bus-routing-match-rules) for mroe details

## Links may be helpful

How to define interface

  - [How to define the xml](https://www.gnu.org/software/emacs/manual/html_node/dbus/Introspection.html)

  - [D-Bus API Design Guidelines](https://dbus.freedesktop.org/doc/dbus-api-design.html)

High level programming of dbus (list of modles you may use, the most intersting one is GDBusProxy)

  - [gdbus-convenience](https://developer.gnome.org/gio/stable/gdbus-convenience.html)
  