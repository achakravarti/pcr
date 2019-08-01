#include "./api.h"


struct pcr_field {
    PCR_FIELD type;
    pcr_string *key;
    void *value;
};


extern pcr_field *pcr_field_new(PCR_FIELD type, const pcr_string *key,
                                        const void *value, pcr_exception ex)
{
}


extern pcr_field *pcr_field_copy(const pcr_field *ctx, pcr_exception ex)
{
}


extern PCR_FIELD pcr_field_type(const pcr_field *ctx, pcr_exception ex)
{
}


extern pcr_string *pcr_field_key(const pcr_field *ctx, pcr_exception ex)
{
}


extern size_t pcr_field_refcount(const pcr_field *ctx, pcr_exception ex)
{
}


extern void *pcr_field_value(const pcr_field *ctx, pcr_exception ex)
{
}


extern void pcr_field_setvalue(pcr_field **ctx, const void *value,
                                    pcr_exception *ex)
{
}

