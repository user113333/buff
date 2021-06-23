struct FONScontext;

enum UIX_COND_ {
    UIX_COND_ONCE = 0b01,
    UIX_COND_ALWAYS = 0b10,
    UIX_COND_ALL = 0b11
};

void uix_context_init(struct FONScontext* fs);
void uix_newframe(void* window);

void uix_next_window_size(float width, float height, enum UIX_COND_ cond);

void uix_begin(const char* name);
void uix_end();

void uix_navbar_begin();
void uix_navbar_end();
int uix_navbar_item(const char* name);

int uix_button(const char* name);

#define UIX_KEY_UNKNOWN            -1
#define UIX_KEY_SPACE              32
#define UIX_KEY_APOSTROPHE         39  /* ' */
#define UIX_KEY_COMMA              44  /* , */
#define UIX_KEY_MINUS              45  /* - */
#define UIX_KEY_PERIOD             46  /* . */
#define UIX_KEY_SLASH              47  /* / */
#define UIX_KEY_0                  48
#define UIX_KEY_1                  49
#define UIX_KEY_2                  50
#define UIX_KEY_3                  51
#define UIX_KEY_4                  52
#define UIX_KEY_5                  53
#define UIX_KEY_6                  54
#define UIX_KEY_7                  55
#define UIX_KEY_8                  56
#define UIX_KEY_9                  57
#define UIX_KEY_SEMICOLON          59  /* ; */
#define UIX_KEY_EQUAL              61  /* = */
#define UIX_KEY_A                  65
#define UIX_KEY_B                  66
#define UIX_KEY_C                  67
#define UIX_KEY_D                  68
#define UIX_KEY_E                  69
#define UIX_KEY_F                  70
#define UIX_KEY_G                  71
#define UIX_KEY_H                  72
#define UIX_KEY_I                  73
#define UIX_KEY_J                  74
#define UIX_KEY_K                  75
#define UIX_KEY_L                  76
#define UIX_KEY_M                  77
#define UIX_KEY_N                  78
#define UIX_KEY_O                  79
#define UIX_KEY_P                  80
#define UIX_KEY_Q                  81
#define UIX_KEY_R                  82
#define UIX_KEY_S                  83
#define UIX_KEY_T                  84
#define UIX_KEY_U                  85
#define UIX_KEY_V                  86
#define UIX_KEY_W                  87
#define UIX_KEY_X                  88
#define UIX_KEY_Y                  89
#define UIX_KEY_Z                  90
#define UIX_KEY_LEFT_BRACKET       91  /* [ */
#define UIX_KEY_BACKSLASH          92  /* \ */
#define UIX_KEY_RIGHT_BRACKET      93  /* ] */
#define UIX_KEY_GRAVE_ACCENT       96  /* ` */
#define UIX_KEY_WORLD_1            161 /* non-US #1 */
#define UIX_KEY_WORLD_2            162 /* non-US #2 */
#define UIX_KEY_ESCAPE             256
#define UIX_KEY_ENTER              257
#define UIX_KEY_TAB                258
#define UIX_KEY_BACKSPACE          259
#define UIX_KEY_INSERT             260
#define UIX_KEY_DELETE             261
#define UIX_KEY_RIGHT              262
#define UIX_KEY_LEFT               263
#define UIX_KEY_DOWN               264
#define UIX_KEY_UP                 265
#define UIX_KEY_PAGE_UP            266
#define UIX_KEY_PAGE_DOWN          267
#define UIX_KEY_HOME               268
#define UIX_KEY_END                269
#define UIX_KEY_CAPS_LOCK          280
#define UIX_KEY_SCROLL_LOCK        281
#define UIX_KEY_NUM_LOCK           282
#define UIX_KEY_PRINT_SCREEN       283
#define UIX_KEY_PAUSE              284
#define UIX_KEY_F1                 290
#define UIX_KEY_F2                 291
#define UIX_KEY_F3                 292
#define UIX_KEY_F4                 293
#define UIX_KEY_F5                 294
#define UIX_KEY_F6                 295
#define UIX_KEY_F7                 296
#define UIX_KEY_F8                 297
#define UIX_KEY_F9                 298
#define UIX_KEY_F10                299
#define UIX_KEY_F11                300
#define UIX_KEY_F12                301
#define UIX_KEY_F13                302
#define UIX_KEY_F14                303
#define UIX_KEY_F15                304
#define UIX_KEY_F16                305
#define UIX_KEY_F17                306
#define UIX_KEY_F18                307
#define UIX_KEY_F19                308
#define UIX_KEY_F20                309
#define UIX_KEY_F21                310
#define UIX_KEY_F22                311
#define UIX_KEY_F23                312
#define UIX_KEY_F24                313
#define UIX_KEY_F25                314
#define UIX_KEY_KP_0               320
#define UIX_KEY_KP_1               321
#define UIX_KEY_KP_2               322
#define UIX_KEY_KP_3               323
#define UIX_KEY_KP_4               324
#define UIX_KEY_KP_5               325
#define UIX_KEY_KP_6               326
#define UIX_KEY_KP_7               327
#define UIX_KEY_KP_8               328
#define UIX_KEY_KP_9               329
#define UIX_KEY_KP_DECIMAL         330
#define UIX_KEY_KP_DIVIDE          331
#define UIX_KEY_KP_MULTIPLY        332
#define UIX_KEY_KP_SUBTRACT        333
#define UIX_KEY_KP_ADD             334
#define UIX_KEY_KP_ENTER           335
#define UIX_KEY_KP_EQUAL           336
#define UIX_KEY_LEFT_SHIFT         340
#define UIX_KEY_LEFT_CONTROL       341
#define UIX_KEY_LEFT_ALT           342
#define UIX_KEY_LEFT_SUPER         343
#define UIX_KEY_RIGHT_SHIFT        344
#define UIX_KEY_RIGHT_CONTROL      345
#define UIX_KEY_RIGHT_ALT          346
#define UIX_KEY_RIGHT_SUPER        347
#define UIX_KEY_MENU               348
#define UIX_KEY_LAST               UIX_KEY_MENU
