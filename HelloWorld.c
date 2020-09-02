#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

    Display *display;
    Window  window;
    XSetWindowAttributes attributes;
    XGCValues gr_values;
    XFontStruct *fontinfo;
    GC gr_context;
    Visual *visual;
    int depth;
    int screen;
    XEvent event;
    XColor    color, dummy;
	XWindowAttributes window_attributes_return;
	

 
int main(int argc, char *argv[])
{
    display = XOpenDisplay(NULL); //To open a connection to the X server that controls a display
    screen = DefaultScreen(display); //returns pointer to the screen
    visual = DefaultVisual(display,screen);
    depth  = DefaultDepth(display,screen);
    attributes.background_pixel = XWhitePixel(display,screen);
 
	/* Create our window */
    window = XCreateWindow( display,XRootWindow(display,screen),
                            200, 200, 350, 200, 5, depth,  InputOutput,
                            visual ,CWBackPixel, &attributes);

	/* X server shall report the events associated with the specified event mask */						
    XSelectInput(display,window,ExposureMask | KeyPressMask); 

	//Set Application name
	char *application_name = "Hello World!";
	XStoreName(display, window, (char *)application_name);
    

	//Set app icon name
	XClassHint *class_hint = XAllocClassHint();
    
    if (class_hint)
    {
        class_hint->res_name = class_hint->res_class = (char *)application_name;
        XSetClassHint(display, window, class_hint);
        XFree(class_hint);
    }

	/* show up window */
	XMapRaised(display, window);

	//Load font
	fontinfo = XLoadQueryFont(display,"6x10");
    XAllocNamedColor(display, DefaultColormap(display, screen),"red",
                      &color,&dummy);
 
    gr_values.font = fontinfo->fid;
    gr_values.foreground = color.pixel;

	/* graphics context and returns a GC. The GC can be used with any destination drawable having the same root and depth as the specified drawable. */
    gr_context = XCreateGC(display,window,GCFont+GCForeground, &gr_values);
    
	/* Maps the window and all of its subwindows that have had map requests.
	*  Mapping a window that has an unmapped ancestor does not display the window 
	*  but marks it as eligible for display when the ancestor becomes mapped. 
	*  Such a window is called unviewable. 
	*  When all its ancestors are mapped, the window becomes viewable and will be visible on the screen if it is not obscured by another window. 
	*  This function has no effect if the window is already mapped. 
	*/
    XMapWindow(display,window);

	
	
	//Our message
	char *msg = "Hello world!";
	
    while(1){
    	XNextEvent(display,&event);

		//Get Windows dimensions
		XGetWindowAttributes(display, window, &window_attributes_return);


		switch(event.type){
        	case Expose: //Expose event (window is visible)

            	XDrawLine(display, window, gr_context, 0, 0, (strlen(msg)*14) - 75, 100 - 5);
            	
				
                
                XDrawString(display, window, gr_context, 100, 100, msg, strlen(msg));
            	
				
				/* ARGUMENTS
				display 	Specifies the connection to the X server.
				d 	Specifies the drawable.
				gc 	Specifies the GC.
				x1  Start position
				y1  Start position
				x2  End Position
				y2  End Position
				*/
				XDrawLine(display, window, gr_context, strlen(msg)*14, 100, window_attributes_return.width, 0);
				
				XDrawLine(display, window, gr_context, (strlen(msg)*14) - 75, 100, 0, window_attributes_return.height);
				XDrawLine(display, window, gr_context, strlen(msg)*14, 100, window_attributes_return.width, window_attributes_return.height);
				
				break;

        	case KeyPress: //When you press any key, program closes
            	XCloseDisplay(display); 
            	exit(0);
        }
    }
}
