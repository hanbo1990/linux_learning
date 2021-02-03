# Dbus example

Example follows [Generating code and docs](https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gio/ch30s05.html#gdbus-example-codegen-server).

The following link may be useful.

[How to define the xml](https://www.gnu.org/software/emacs/manual/html_node/dbus/Introspection.html)

[D-Bus API Design Guidelines](https://dbus.freedesktop.org/doc/dbus-api-design.html)

You can find more detail about dbus on [gdbus-convenience](https://developer.gnome.org/gio/stable/gdbus-convenience.html)

## Server code

Follow [this link](./01_server/README.md).

## Client code from generated code

Client code using generated code.

Follow [this link](./02_client_generated_code) shows client code using the generated code from example.xml.

Client code using proxy not from generated code but using GDBusProxy class.

Follow [this link](./03_client_proxy/README.md) for more detail.

## Debug

if we want to listen to signal from "com.test.animal", we can use `dbus-monitor "type=signal,sender='com.test.animal'"` as an example.

Check [match rule](https://dbus.freedesktop.org/doc/dbus-specification.html#message-bus-routing-match-rules) for mroe details
