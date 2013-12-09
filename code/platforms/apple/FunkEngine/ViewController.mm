#import "ViewController.h"

#include <core/Core.h>
#include <system/Window.h>
#include <common/Debug.h>
#include <system/TouchInputManager.h>

#define TOUCH_FLIP_Y

using namespace funk;

@interface ViewController()
{
	funk::Core m_core;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)setupWindow;
- (void)tearDownGL;
- (funk::TouchInput)BuildTouchInput:(UITouch*) touch;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setupGL];
	[self setupWindow];
	m_core.Init(self.framesPerSecond);
}

- (void)viewWillAppear
{
	printf("width: %f, height: %f¥n",  self.view.window.bounds.size.width,  self.view.window.bounds.size.height );
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
	ERROR_MSG("Memory Warning Received!");
	
    [super didReceiveMemoryWarning];
	
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
}

-(NSUInteger)supportedInterfaceOrientations
{
	// lock orientation to landscape!! (for now)
	return UIInterfaceOrientationMaskLandscape;
}

- (void)setupWindow
{
	self.view.multipleTouchEnabled = YES;
	
	// setup window
	funk::Window::CreateInst();
	
	// screen
	UIInterfaceOrientation orient = self.interfaceOrientation;
	int width = self.view.bounds.size.width;
	int height = self.view.bounds.size.height;
	if ( orient == UIInterfaceOrientationLandscapeLeft || orient == UIInterfaceOrientationLandscapeRight )
	{
		width = self.view.bounds.size.height;
		height = self.view.bounds.size.width;
	}
	
	// retina sccaling
	CGFloat scale = [UIScreen mainScreen].scale;
	funk::Window::Ref().SetDimen( width*scale, height*scale );
	funk::Window::Ref().SetDimenMonitor( width*scale, height*scale );
}

- (void)setupGL
{
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	self.preferredFramesPerSecond = 60;
	
    [EAGLContext setCurrentContext:self.context];
	
    if (!self.context)
	{
        NSLog(@"Failed to create ES context");
		ERROR_MSG("Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	printf("from - width: %f, height: %f, %d\n",  self.view.window.bounds.size.width,  self.view.window.bounds.size.height , fromInterfaceOrientation );
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	//printf("to- width: %f, height: %f, %d\n",  self.view.bounds.size.width,  self.view.window.bounds.size.height, toInterfaceOrientation );
	
	int width = self.view.bounds.size.width;
	int height = self.view.bounds.size.height;
	funk::DeviceOrientation orientation = funk::DEVICE_ORIENTATION_PORTRAIT;
	
	// if landscape mode, must change window size
	if ( toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight )
	{
		width = self.view.bounds.size.height;
		height = self.view.bounds.size.width;
		orientation = funk::DEVICE_ORIENTATION_LANDSCAPE;
	}
	
	funk::Window::Ref().OnRotateOrientation( orientation, width, height );
}

- (void)update
{
	m_core.Update( self.timeSinceLastUpdate );
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	m_core.Draw( self.timeSinceLastDraw );
}

- (funk::TouchInput)BuildTouchInput:(UITouch*) touch
{
	CGPoint pos = [touch previousLocationInView:self.view];
	CGPoint pos_prev = [touch locationInView:self.view];
	
	funk::TouchEventType touch_state;
	switch( touch.phase )
	{
		case UITouchPhaseBegan		: touch_state = TOUCHEVENT_BEGIN; break;
		case UITouchPhaseMoved		: touch_state = TOUCHEVENT_MOVE; break;
		case UITouchPhaseStationary : touch_state = TOUCHEVENT_STATIONARY; break;
		case UITouchPhaseEnded		: touch_state = TOUCHEVENT_END; break;
		case UITouchPhaseCancelled	: touch_state = TOUCHEVENT_CANCEL; break;
		default: ERROR_MSG("Unrecognized touch state");
	};
	
	funk::TouchInput out_touch;
	out_touch.pos			= v2( pos.x, pos.y );
	out_touch.pos_start		= v2( pos.x, pos.y );
	out_touch.pos_prev		= v2( pos_prev.x, pos_prev.y );
	out_touch.tap_count		= touch.tapCount;
	out_touch.timestamp		= touch.timestamp;
	out_touch.id			= (uint32_t)touch;
	out_touch.state			= touch_state;	
	
	// invert positions of touch
	out_touch.pos.y = self.view.bounds.size.height - pos.y;
	out_touch.pos_prev.y = self.view.bounds.size.height - pos_prev.y;
	
	// handle retina window scaling
	CGFloat scale = [UIScreen mainScreen].scale;
	out_touch.pos *= scale;
	out_touch.pos_prev *= scale;
	
	return out_touch;
}

// touch events
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesBegan:touches withEvent:event];
	
	funk::TouchInputManager & touch_mngr = funk::TouchInputManager::Ref();
	for (UITouch *curr_touch in touches)
	{
		funk::TouchInput touch = [self BuildTouchInput:curr_touch];
		touch_mngr.OnEventTouchBegin( touch );
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesMoved:touches withEvent:event];
	
	funk::TouchInputManager & touch_mngr = funk::TouchInputManager::Ref();
	for (UITouch *curr_touch in touches)
	{
		funk::TouchInput touch = [self BuildTouchInput:curr_touch];
		touch_mngr.OnEventTouchMove( touch );
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesEnded:touches withEvent:event];
	
	funk::TouchInputManager & touch_mngr = funk::TouchInputManager::Ref();
	for (UITouch *curr_touch in touches)
	{
		funk::TouchInput touch = [self BuildTouchInput:curr_touch];
		touch_mngr.OnEventTouchEnd( touch );
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesCancelled:touches withEvent:event];
	
	funk::TouchInputManager & touch_mngr = funk::TouchInputManager::Ref();
	for (UITouch *curr_touch in touches)
	{
		funk::TouchInput touch = [self BuildTouchInput:curr_touch];
		touch_mngr.OnEventTouchCancel( touch );
    }
}

@end
