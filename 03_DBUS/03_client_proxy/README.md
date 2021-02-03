# Dbus Client Proxy

The minimal required class is `GDBusProxy`. Follow the [GIO Reference manual](https://developer.gnome.org/gio/stable/GDBusProxy.html) to check more details. You can find almost all the functionality you need there.

## Steps

1. Create the proxy, you can use `g_dbus_proxy_new_sync` for example.
2. If you already know what filed is supported in remote sever, use proxy to:
  - call method by using function like `g_dbus_proxy_call`
  - set/get property
  - subscribe to signal like `g-properties-changed` supported by the class
  - subscribe to signal to remote object using function like `g_dbus_connection_signal_subscribe`
3. you need to clean up the proxy, connection ..etc when finish.

## Other

You can also try to watch the dbus name, and create proxy when it appears, and delete the proxy when the name disappears. Which make it easier to manage the proxy.