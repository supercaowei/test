#include <stdio.h>

typedef struct {
    unsigned char c;
} B;

/**注意：LogFieldIndex中的字段定义双端要保持一致，不可单端独自修改。**/
enum LogFieldIndex {
    message, //0
    adm,
    avo_cache_info,
    transport_layer_status,
    video_filter,
    url,
    video_frame_elapse,
    camera,
    rms,
    adm_status,
    adm_pts_info,
    transport_send_stall,
    push_stream_stall_status,
    roi,
    end
};

struct FieldMask {
    int mask;
    const char field[32];
};

#define ENUM_FIELD_MASK(fieldName) {1 << (int)fieldName, #fieldName}

static struct FieldMask logFields[] = {
    ENUM_FIELD_MASK(message),
    ENUM_FIELD_MASK(adm),
    ENUM_FIELD_MASK(avo_cache_info),
    ENUM_FIELD_MASK(transport_layer_status),
    ENUM_FIELD_MASK(video_filter),
    ENUM_FIELD_MASK(url),
    ENUM_FIELD_MASK(video_frame_elapse),
    ENUM_FIELD_MASK(camera),
    ENUM_FIELD_MASK(rms),
    ENUM_FIELD_MASK(adm_status),
    ENUM_FIELD_MASK(adm_pts_info),
    ENUM_FIELD_MASK(transport_send_stall),
    ENUM_FIELD_MASK(push_stream_stall_status),
    ENUM_FIELD_MASK(roi),
};


int main()
{
    // B b;
	// printf("%lu\n", sizeof(b));
    printf("%lu, %f, %f, %f\n", sizeof(float), (float)(__FLT_MAX__), (float)(-1 * __FLT_MAX__), (float)(__FLT_MIN__));
	return 0;
}
