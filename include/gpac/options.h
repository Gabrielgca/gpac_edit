/*
 *			GPAC - Multimedia Framework C SDK
 *
 *			Authors: Jean Le Feuvre
 *			Copyright (c) Telecom ParisTech 2000-2022
 *					All rights reserved
 *
 *  This file is part of GPAC / Stream Management sub-project
 *
 *  GPAC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  GPAC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */



#ifndef _GF_OPTIONS_H_
#define _GF_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif


/*!
\file <gpac/options.h>
\brief Compositor and terminal options.
*/
	
/*!
\addtogroup compose_grp Compositor
\ingroup playback_grp
\brief GPAC A/V/2D/3D compositor/rendering.

This section documents the compositor of GPAC? in charge of assembling audio, images, video, text, 2D and 3D graphics with in a timed way

@{
 */
	
/*! AspectRatio Type */
enum
{
	 /*! keep AR*/
	GF_ASPECT_RATIO_KEEP = 0,
	/*! keep 16/9*/
	GF_ASPECT_RATIO_16_9,
	/*! keep 4/3*/
	GF_ASPECT_RATIO_4_3,
	/*! none (all rendering area used)*/
	GF_ASPECT_RATIO_FILL_SCREEN
};

/*! AntiAlias settings*/
enum
{
	/*! no antialiasing*/
	GF_ANTIALIAS_NONE = 0,
	/*! only text has antialiasing*/
	GF_ANTIALIAS_TEXT,
	/*! full antialiasing*/
	GF_ANTIALIAS_FULL
};

/*! PlayState settings*/
enum
{
	/*! terminal is playing (get only)*/
	GF_STATE_PLAYING = 0,
	/*! terminal is paused (get only)*/
	GF_STATE_PAUSED,
	/*! On set, terminal will pause after next frame (simulation tick). On get, indicates that rendering step hasn't performed yet*/
	GF_STATE_STEP_PAUSE,
	/*! indicates resume shall restart from live point if any rather than pause point (set only)*/
	GF_STATE_PLAY_LIVE = 10
};

/*! interaction level settings
\note GF_INTERACT_NORMAL and GF_INTERACT_NAVIGATION filter events. If set, any event processed by these 2 modules won't be forwarded to the user
*/
enum
{
	/*! regular interactions enabled (touch sensors)*/
	GF_INTERACT_NORMAL = 1,
	/*! InputSensor interactions enabled (mouse and keyboard)*/
	GF_INTERACT_INPUT_SENSOR = 2,
	/*! all navigation interactions enabled (mouse and keyboard)*/
	GF_INTERACT_NAVIGATION = 4,
};

/*! BoundingVolume settings*/
enum
{
	/*! doesn't draw bounding volume*/
	GF_BOUNDS_NONE = 0,
	/*! draw object bounding box / rect*/
	GF_BOUNDS_BOX,
	/*! draw object AABB tree (3D only) */
	GF_BOUNDS_AABB
};

/*! Wireframe settings*/
enum
{
	/*! draw solid volumes*/
	GF_WIREFRAME_NONE = 0,
	/*! draw only wireframe*/
	GF_WIREFRAME_ONLY,
	/*! draw wireframe on solid object*/
	GF_WIREFRAME_SOLID
};


/*! navigation type*/
enum
{
	/*! navigation is disabled by content and cannot be forced by user*/
	GF_NAVIGATE_TYPE_NONE,
	/*! 2D navigation modes only can be used*/
	GF_NAVIGATE_TYPE_2D,
	/*! 3D navigation modes only can be used*/
	GF_NAVIGATE_TYPE_3D
};

/*! navigation modes - non-VRML ones are simply blaxxun contact ones*/
enum
{
	/*! no navigation*/
	GF_NAVIGATE_NONE = 0,
	/*! 3D navigation modes*/
	/*! walk navigation*/
	GF_NAVIGATE_WALK,
	/*! fly navigation*/
	GF_NAVIGATE_FLY,
	/*! pan navigation*/
	GF_NAVIGATE_PAN,
	/*! game navigation*/
	GF_NAVIGATE_GAME,
	/*! slide navigation, for 2D and 3D*/
	GF_NAVIGATE_SLIDE,
	/*! all modes below disable collision detection & gravity in 3D*/
	/*examine navigation, for 2D and 3D */
	GF_NAVIGATE_EXAMINE,
	/*! orbit navigation - 3D only*/
	GF_NAVIGATE_ORBIT,
	/*! QT-VR like navigation - 3D only*/
	GF_NAVIGATE_VR,
};

/*! collision flags*/
enum
{
	/*! no collision*/
	GF_COLLISION_NONE,
	/*! regular collision*/
	GF_COLLISION_NORMAL,
	/*! collision with camera displacement*/
	GF_COLLISION_DISPLACEMENT,
};

/*! TextTexturing settings*/
enum
{
	/*! text drawn as texture in 3D mode, regular in 2D mode*/
	GF_TEXTURE_TEXT_DEFAULT = 0,
	/*! text never drawn as texture*/
	GF_TEXTURE_TEXT_NEVER,
	/*! text always drawn*/
	GF_TEXTURE_TEXT_ALWAYS
};

/*! Normal drawing settings*/
enum
{
	/*! normals never drawn*/
	GF_NORMALS_NONE = 0,
	/*! normals drawn per face (at barycenter)*/
	GF_NORMALS_FACE,
	/*! normals drawn per vertex*/
	GF_NORMALS_VERTEX
};


/*! Back-face culling mode*/
enum
{
	/*! backface culling disabled*/
	GF_BACK_CULL_OFF = 0,
	/*! backface culliong enabled*/
	GF_BACK_CULL_ON,
	/*! backface culling enabled also for transparent meshes*/
	GF_BACK_CULL_ALPHA,
};

/*! 2D drawing mode*/
enum
{
	/*! defer draw mode (only modified parts of the canvas are drawn)*/
	GF_DRAW_MODE_DEFER=0,
	/*! immediate draw mode (canvas always redrawn)*/
	GF_DRAW_MODE_IMMEDIATE,
	/*! defer debug draw mode (only modified parts of the canvas are drawn, the rest of the canvas is erased)*/
	GF_DRAW_MODE_DEFER_DEBUG,
};

/*! high-level options*/
enum
{
	/*! set/get antialias flag (value: one of the AntiAlias enum) - may be ignored in OpenGL mode depending on graphic cards*/
	GF_OPT_ANTIALIAS  =0,
	/*! set/get fast mode (value: boolean) */
	GF_OPT_HIGHSPEED,
	/*! set/get fullscreen flag (value: boolean) */
	GF_OPT_FULLSCREEN,
	/*! reset top-level transform to original (value: boolean)*/
	GF_OPT_ORIGINAL_VIEW,
	/*! overrides BIFS size info for simple AV - this is not recommended since
	it will resize the window to the size of the biggest texture (thus some elements
	may be lost)*/
	GF_OPT_OVERRIDE_SIZE,
	/*! set / get audio volume (value is intensity between 0 and 100) */
	GF_OPT_AUDIO_VOLUME,
	/*! set / get audio pan (value is pan between 0 (all left) and 100(all right) )*/
	GF_OPT_AUDIO_PAN,
	/*! set / get audio mute*/
	GF_OPT_AUDIO_MUTE,
	/*! get javascript flag (no set, depends on compil) - value: boolean, true if JS enabled in build*/
	GF_OPT_HAS_JAVASCRIPT,
	/*! get selectable stream flag (no set) - value: boolean, true if audio/video/subtitle stream selection is
	possible with content (if an MPEG-4 scene description is not present). Use regular OD browsing to get streams*/
	GF_OPT_CAN_SELECT_STREAMS,
	/*! set/get control interaction, OR'ed combination of interaction flags*/
	GF_OPT_INTERACTION_LEVEL,
	/*! set display window visible / get show/hide state*/
	GF_OPT_VISIBLE,
	/*! set freeze display on/off / get freeze state freeze_display prevents any screen updates
	needed when output driver uses direct video memory access*/
	GF_OPT_FREEZE_DISPLAY,
	/*! Returns 1 if file playback is considered as done (all streams finished, no active time sensors
	and no user interactions in the scene)*/
	GF_OPT_IS_FINISHED,
	/*! Returns 1 if file timeline is considered as done (all streams finished, no active time sensors)*/
	GF_OPT_IS_OVER,
	/*! set/get aspect ratio (value: one of AspectRatio enum) */
	GF_OPT_ASPECT_RATIO,
	/*! send a redraw message (SetOption only): all graphics info (display list, vectorial path) is
	recomputed, and textures are reloaded in HW*/
	GF_OPT_REFRESH,
	/*! set/get stress mode (value: boolean) - in stress mode a GF_OPT_FORCE_REDRAW is emulated at each frame*/
	GF_OPT_STRESS_MODE,
	/*! get/set bounding volume drawing (value: one of the above option)*/
	GF_OPT_DRAW_BOUNDS,
	/*! get/set texture text option - when enabled and usable (that depends on content), text is first rendered
	to a texture and only the texture is drawn, rather than drawing all the text each time (CPU intensive)*/
	GF_OPT_TEXTURE_TEXT,
	/*! reload config file (set only), including drivers. Plugins configs are not reloaded*/
	GF_OPT_RELOAD_CONFIG,
	/*! get: returns whether the content enable navigation and if it's 2D or 3D.
	set: reset viewpoint (whatever value is given)*/
	GF_OPT_NAVIGATION_TYPE,
	/*! get current navigation mode - set navigation mode if allowed by content - this is not a resident
	option (eg not stored in cfg)*/
	GF_OPT_NAVIGATION,
	/*! get/set Play state - cf above states for set*/
	GF_OPT_PLAY_STATE,
	/*! get only: returns 1 if main addon is playing, 0 if regular scene is playing*/
	GF_OPT_MAIN_ADDON,
	/*! get/set bench mode - if enabled, video frames are drawn as soon as possible witthout checking synchronisation*/
	GF_OPT_VIDEO_BENCH,
	/*! get/set OpenGL force mode - returns error if OpenGL is not supported*/
	GF_OPT_USE_OPENGL,
	/*! set/get draw mode*/
	GF_OPT_DRAW_MODE,
	/*! set/get scalable zoom (value: boolean)*/
	GF_OPT_SCALABLE_ZOOM,
	/*! set/get YUV acceleration (value: boolean) */
	GF_OPT_YUV_HARDWARE,
	/*! get (set not supported yet) hardware YUV format (value: YUV 4CC) */
	GF_OPT_YUV_FORMAT,
	/*! max video cache size in kbytes*/
	GF_OPT_VIDEO_CACHE_SIZE,
	/*! max HTTP download rate in bits per second, 0 if no limit*/
	GF_OPT_HTTP_MAX_RATE,
	/*! set only (value: boolean). If set, the main audio mixer can no longer be reconfigured. */
	GF_OPT_FORCE_AUDIO_CONFIG,

	/*!		3D ONLY OPTIONS		*/
	/*! set/get raster outline flag (value: boolean) - when set, no vectorial outlining is done, only OpenGL raster outline*/
	GF_OPT_RASTER_OUTLINES,
	/*! set/get pow2 emulation flag (value: boolean) - when set, video textures with non power of 2 dimensions
	are emulated as pow2 by expanding the video buffer (image is not scaled). Otherwise the entire image
	is rescaled. This flag does not affect image textures, which are always rescaled*/
	GF_OPT_EMULATE_POW2,
	/*! get/set polygon antialiasing flag (value: boolean) (may be ugly with some cards)*/
	GF_OPT_POLYGON_ANTIALIAS,
	/*! get/set wireframe flag (value: cf above) (may be ugly with some cards)*/
	GF_OPT_WIREFRAME,
	/*! get/set wireframe flag (value: cf above) (may be ugly with some cards)*/
	GF_OPT_NORMALS,
	/*! disable backface culling*/
	GF_OPT_BACK_CULL,
	/*! get/set RECT Ext flag (value: boolean) - when set, GL rectangular texture extension is not used
	(but NPO2 texturing is if available)*/
	GF_OPT_NO_RECT_TEXTURE,
	/*! set/get headlight (value: boolean)*/
	GF_OPT_HEADLIGHT,
	/*! set/get collision (value: cf above)*/
	GF_OPT_COLLISION,
	/*! set/get gravity*/
	GF_OPT_GRAVITY,
	/*! get the number of offscreen views in stereo mode, or 1 if no offscreen stereo views are available*/
	GF_OPT_NUM_STEREO_VIEWS,
	/*! set the mode of display of HEVC multiview videos, 0 to display the two views/layers and 1 to display just the first view/layer*/
	GF_OPT_MULTIVIEW_MODE,
	/*! get orientation sensors flag, true if sensors are activated false if not*/
	GF_OPT_ORIENTATION_SENSORS_ACTIVE,
};

/*! @} */

#ifdef __cplusplus
}
#endif

#endif	/*_GF_USER_H_*/

