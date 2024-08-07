/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
// #include "/home/mushroom/.cache/wal/colors-wal-dwm.h"
static unsigned int borderpx = 1;    /* border pixel of windows */
static unsigned int snap = 32;       /* snap pixel */
static int showbar = 1;              /* 0 means no bar */
static int topbar = 1;               /* 0 means bottom bar */
static const unsigned int gappx = 5; /* gaps between windows */
static const int swallowfloating =
    0; /* 1 means swallow floating windows by default */
static const char *fonts[] = {
    "SourceCodePro-Medium:size=11",
    "JoyPixels:pixelsize=11:antialias=true:autohint=true"};
static const char dmenufont[] = "monospace:size=10";
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};
// static const char *colors[][3] = {
/*               fg         bg         border   */
/* [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
[SchemeSel] = {col_gray4, col_cyan, col_cyan}, */
// } */;

/* tagging */
// static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const char *tags[] = {"", "", "3", "4", "5", "6"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"St", NULL, NULL, 0, 0, 1, 0, -1},
    {"feh", NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, /* xev */
};

/* layout(s) */
static float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static int nmaster = 1;     /* number of clients in master area */
static int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"~/.config/rofi/launchers/type-6/launcher.sh",
                                 NULL};

static const char *roficmd[] = {"rofi",
                                "-show",
                                "drun",
                                "-theme",
                                "~/.config/rofi/launchers/type-6/style-9.rasi",
                                NULL};
static const char *termcmd[] = {"kitty", NULL};

static const char *screenshot_full_screen[] = {
    "scrot", "-q", "1",
    "/home/mushroom/Pictures/screenshots/%Y-%m-%d-%H:%M:%S_$wx$h.png", NULL};

static const char *screenshot_selection[] = {
    "scrot", "-s",
    "/home/mushroom/Pictures/screenshots/%Y-%m-%d-%H:%M:%S_$wx$h.png", NULL};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"normbgcolor", STRING, &normbgcolor},
    {"normbordercolor", STRING, &normbordercolor},
    {"normfgcolor", STRING, &normfgcolor},
    {"selbgcolor", STRING, &selbgcolor},
    {"selbordercolor", STRING, &selbordercolor},
    {"selfgcolor", STRING, &selfgcolor},
};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = roficmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY | ShiftMask, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    // For volume controls
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("pamixer -i 2; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("pamixer -d 2; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioMute, spawn,
     SHCMD("pamixer -t; pkill -RTMIN+10 dwmblocks")},

    // For display brightness
    {0, XF86XK_MonBrightnessDown, spawn,
     SHCMD("light -U 2; pkill -RTMIN+20 dwmblocks")},
    {0, XF86XK_MonBrightnessUp, spawn,
     SHCMD("light -A 2; pkill -RTMIN+20 dwmblocks")},

    // slock
    {MODKEY, XK_x, spawn, SHCMD("slock")},

    // Keepassxc
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("keepassxc")},

    // Powermenu
    {MODKEY | ControlMask, XK_p, spawn, SHCMD("~/.config/rofi/powermenu.sh")},

    // Brave
    {MODKEY, XK_b, spawn, SHCMD("brave")},

    // for screenshots
    {0, XK_Print, spawn, {.v = screenshot_full_screen}},
    {ControlMask, XK_Print, spawn, {.v = screenshot_selection}},

    // For gaps
    {MODKEY, XK_minus, setgaps, {.i = -1}},
    {MODKEY, XK_equal, setgaps, {.i = +1}},
    {MODKEY | ShiftMask, XK_equal, setgaps, {.i = 0}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5){MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
    /* signum       function        argument  */
    {1, reload_xresources, {.v = NULL}},
};
