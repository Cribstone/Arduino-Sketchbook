/* Copyright (c) 2011, Peter Barrett  
 **  
 ** Permission to use, copy, modify, and/or distribute this software for  
 ** any purpose with or without fee is hereby granted, provided that the  
 ** above copyright notice and this permission notice appear in all copies.  
 ** 
 ** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
 ** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
 ** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
 ** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
 ** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
 ** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
 ** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
 ** SOFTWARE.  
 */

#include "Platform.h"
#include "HIDWiiClassic.h"

#if defined(USBCON)
#ifdef HID_ENABLED

HIDWiiClassic_ HIDWiiClassic;

// HID report descriptor

#define RAWHID_USAGE_PAGE 0xFFC0
#define RAWHID_USAGE 0x0C00
#define RAWHID_TX_SIZE 64
#define RAWHID_RX_SIZE 64

extern const u8 _hidReportDescriptor[] PROGMEM;
// HID information from http://www.instructables.com/id/USB-Wii-Classic-Controller/
const u8 _hidReportDescriptor[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (16)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x33,                    //     USAGE (Rx)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};
extern const HIDDescriptor _hidInterface PROGMEM;
const HIDDescriptor _hidInterface =
{
    D_INTERFACE(HID_INTERFACE,1,3,0,0),
    D_HIDREPORT(sizeof(_hidReportDescriptor)),
    D_ENDPOINT(USB_ENDPOINT_IN (HID_ENDPOINT_INT),USB_ENDPOINT_TYPE_INTERRUPT,0x40,0x01)
};

//================================================================================
//================================================================================
// Driver

u8 _hid_protocol = 1;
u8 _hid_idle = 1;

#define WEAK __attribute__ ((weak))
#define WEAK 

int WEAK HID_GetInterface(u8* interfaceNum)
{
    interfaceNum[0] += 1;	// uses 1
    return USB_SendControl(TRANSFER_PGM,&_hidInterface,sizeof(_hidInterface));
}

int WEAK HID_GetDescriptor(int i)
{
    return USB_SendControl(TRANSFER_PGM,_hidReportDescriptor,sizeof(_hidReportDescriptor));
}

void WEAK HID_SendReport(u8 id, const void* data, int len)
{
    USB_Send(HID_TX, &id, 1);
    USB_Send(HID_TX | TRANSFER_RELEASE,data,len);
}

bool WEAK HID_Setup(Setup& setup)
{
    u8 r = setup.bRequest;
    u8 requestType = setup.bmRequestType;
    if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType)
    {
        if (HID_GET_REPORT == r)
        {
            //HID_GetReport();
            return true;
        }
        if (HID_GET_PROTOCOL == r)
        {
            //Send8(_hid_protocol);	// TODO
            return true;
        }
    }

    if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
    {
        if (HID_SET_PROTOCOL == r)
        {
            _hid_protocol = setup.wValueL;
            return true;
        }

        if (HID_SET_IDLE == r)
        {
            _hid_idle = setup.wValueL;
            return true;
        }
    }
    return false;
}

/*
 * HIDWiiClassic
 * Usage: HIDWiiClassic.write(keys)
 * Fill in keys using the constants defined in HIDWiiClassic.h
 */

HIDWiiClassic_::HIDWiiClassic_()
{
}

void HIDWiiClassic_::begin(void)
{
}

void HIDWiiClassic_::end(void)
{
}

void HIDWiiClassic_::write(HIDWiiClassicKeys &keys)
{
    //u8 j[6] = {
        //4,
        //6,
        ////keys.buttons & 0x00FF,
        ////keys.buttons >> 16,
        //keys.left_x,
        //keys.left_y,
        //keys.right_x,
        //-12//keys.right_y
    //};
    //j = (u8) keys;
    //HID_SendReport(Report number, array of values in same order as HID descriptor, length)
    HID_SendReport(1, &keys, sizeof(HIDWiiClassicKeys));
}

#endif

#endif /* if defined(USBCON) */
