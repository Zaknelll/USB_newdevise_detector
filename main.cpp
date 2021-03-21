#include <iostream>

using namespace std;

#include <stdio.h>
#include <usb.h>
#include <libusb-1.0/libusb.h>
#include <cassert>

bool discoverDevices();

bool is_interesting(libusb_device *pDevice);

int main() {
    struct usb_bus *bus;
    struct usb_device *dev;
//    usb_init();
//    usb_find_busses();
//    usb_find_devices();
//    int currDevNum = sizeof (usb_busses);
//
//    for (bus = usb_busses; bus; bus = bus->next) {
//        for (dev = bus->devices; dev; dev = dev->next) {
//            printf("Trying device %s/%s\n", bus->dirname, dev->filename);
//            printf("\tID_VENDOR = 0x%04x\n", dev->descriptor.idVendor);
//            printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct);
//        }
//    }

//    printf("After cycle\n");

    bool result = discoverDevices();
//    cout << result;

}

bool discoverDevices(){
    // discover devices
//    printf("Func Start\n");
    libusb_device **list;
    libusb_device *found = NULL;
    libusb_context *context = NULL;
    int result = libusb_init(&context);
    if (0 > result)
    {
        fprintf(stderr, "libusb_init() failed with %d.\n", result);
        exit(EXIT_FAILURE);
    }
    ssize_t cnt = libusb_get_device_list(context, &list);
    ssize_t i = 0;
    int err = 0;
    if (cnt < 0) {
        cout << "Error:" << cnt  << endl;
        return false;
    }

    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        if (is_interesting(device)) {
            found = device;
            break;
        }
    }

    if (found) {
        libusb_device_handle *handle;

        printf("Trying to open device!\n");
        err = libusb_open(found, &handle);
        if (err) {
            cout << "Error while open" << endl;
            return false;
        }
        // etc
    }

    libusb_free_device_list(list, 1);

    libusb_exit(context);

    return true;

}

bool is_interesting(libusb_device *pDevice) {
    libusb_device_descriptor desc = {0};
    int rc = 0;


    rc = libusb_get_device_descriptor(pDevice, &desc);
    assert(rc == 0);



    if (desc.idProduct == 0x5567) {
        printf("Vendor:Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
        return true;
    }

    return false;
}
