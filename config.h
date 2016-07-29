לפי דעתי מימוש הפונקציות בקובץ יהיה כזה:
typedef struct sp_config_t* SPConfig; זה יהיה המבנה שיחזיק את הערכים שנמצאים בקובץ הקונפיג במשתנים פנימיים שלו
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg); זאת הפונקציה המאתגרת שתיצור את המבנה הזה מקוב טקסט נתון
void spConfigDestroy(SPConfig config); הורס את המבנה
שאר הפונקציות שכתובות בהדר יחזירו משתנים ספציפיים מהמבנה.
