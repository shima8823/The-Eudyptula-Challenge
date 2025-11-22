#include <linux/module.h>
#include <linux/printk.h>
#include <linux/usb.h>
#include <linux/hid.h>
#include <linux/usb/ch9.h>

static struct usb_device_id skel_table[] = {
	// {  USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
	//     USB_INTERFACE_SUBCLASS_BOOT,
	//     USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{USB_INTERFACE_INFO(USB_CLASS_AUDIO,
			0x02,
			0)},
	{}
};

static int skel_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "[skel] Skeleton Driver (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);
    return 0;
}	

static void skel_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "[skel] Skeleton Driver removed\n");
}

static struct usb_driver skel_driver = {
	.name = "skel_driver",
	.id_table = skel_table,
	.probe = skel_probe,
	.disconnect = skel_disconnect,
};

module_usb_driver(skel_driver);

MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(usb, skel_table);
MODULE_AUTHOR("shshimad");
MODULE_DESCRIPTION("A Simple Skeleton Driver");    