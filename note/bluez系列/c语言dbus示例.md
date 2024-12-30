gcc -O2 $(pkg-config --cflags glib-2.0 json-glib-1.0) -o test-gdbus test-gdbus.c $(pkg-config --libs glib-2.0 json-glib-1.0)

```
gcc -O2 $(pkg-config --cflags glib-2.0 json-glib-1.0) -o test-gdbus test-gdbus.c $(pkg-config --libs glib-2.0 json-glib-1.0)
```

