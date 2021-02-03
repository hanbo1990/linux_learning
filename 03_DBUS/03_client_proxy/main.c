#include <gio/gio.h>

#define TARGET_SERVER_NAME      "com.test.animal"
#define TARGET_SERVER_OBJ_PATH  "/com/test/animal/001"
#define TARGET_SERVER_INTERFACE "org.gtk.GDBus.Example.ObjectManager.Animal"
#define TARGET_SIG_NAME_JUMP    "Jumped"
#define TARGET_METHOD_POKE      "Poke"

static void signal_received(GDBusConnection* connection, const gchar* sender_name, const gchar* object_path,
                            const gchar* interface_name, const gchar* signal_name, GVariant* parameters,
                            gpointer user_data)
{
    g_print("signal received %s\n", signal_name);
}

static void on_call_done(GObject* source, GAsyncResult* res, gpointer usr_data)
{
    GError* error = NULL;
    g_dbus_proxy_call_finish((GDBusProxy*) source, res, &error);
    if (NULL != error) {
        g_print("failed to call with error %s\n", error->message);
    }
    g_print("call done\n");
}

static void connection_connect_signal(GDBusProxy* proxy)
{
    // return code not used in testing code
    g_dbus_connection_signal_subscribe(g_dbus_proxy_get_connection(proxy), g_dbus_proxy_get_name(proxy),
                                       g_dbus_proxy_get_interface_name(proxy), TARGET_SIG_NAME_JUMP,
                                       g_dbus_proxy_get_object_path(proxy), NULL, G_DBUS_SIGNAL_FLAGS_NONE,
                                       signal_received, NULL, NULL);
}

static void properity_changed(GDBusProxy* proxy, GVariant* changed_properties, GStrv invalidated_properties,
                              gpointer user_data)
{
    g_print("Properities changed\n");
}

static void call_function(GDBusProxy* proxy)
{
    g_dbus_proxy_call(proxy, TARGET_METHOD_POKE, g_variant_new("(bb)", 1, 0), G_DBUS_CALL_FLAGS_NONE, -1, NULL,
                      on_call_done, NULL);
}

gint main(gint argc, char** argv)
{
    GDBusConnection* con = NULL;
    GDBusProxy* proxy = NULL;
    GMainLoop* loop = NULL;
    GError* error = NULL;

    loop = g_main_loop_new(NULL, FALSE);
    if (NULL == loop) {
        g_error("failed to create gmain loop!\n");
        return 1;
    }

    con = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
    if (NULL != error) {
        g_error("failed to get session bus with msg: %s\n", error->message);
        return 1;
    }

    proxy = g_dbus_proxy_new_sync(con, G_DBUS_PROXY_FLAGS_DO_NOT_CONNECT_SIGNALS, NULL, TARGET_SERVER_NAME,
                                  TARGET_SERVER_OBJ_PATH, TARGET_SERVER_INTERFACE, NULL, &error);

    if (NULL != error) {
        g_error("failed to get proxy \n");
        return 1;
    }

    // not freed, testing code don't care about memory leakage....!
    g_print("name for proxy is %s\n", g_dbus_proxy_get_name(proxy));
    g_print("name owner for proxy is %s\n", g_dbus_proxy_get_name_owner(proxy));   // g_free should be used
    g_print("object path for proxy is %s\n", g_dbus_proxy_get_object_path(proxy));
    g_print("interface name for proxy is %s\n", g_dbus_proxy_get_interface_name(proxy));
    g_print("default timeout for proxy is %d\n", g_dbus_proxy_get_default_timeout(proxy));
    g_print("cached properities for proxy is %s\n",
            *g_dbus_proxy_get_cached_property_names(proxy));   // leakage here, g_strfreev

    // try signal
    connection_connect_signal(proxy);

    // check what signal this class has
    g_signal_connect(proxy, "g-properties-changed", G_CALLBACK(properity_changed), NULL);

    call_function(proxy);

    g_main_loop_run(loop);

    return 0;
}