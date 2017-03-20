/*
 * $Id$
 * Yonx <xuy1202@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


void hijack_call(field_info *fi)
{
    if(! fi) return;
    const char* finfo_name   = fi->hfinfo->name;
    const char* finfo_abbrev = fi->hfinfo->abbrev;
    int finfo_type = (fi->hfinfo) ? fi->hfinfo->type : FT_NONE;
    switch (finfo_type) {
        case FT_NONE:
            break;
        case FT_PROTOCOL:
            printf("hijack_call: Yes\n");
            break;
        case FT_UINT_BYTES:
        case FT_BYTES:
            {
                guint8* bytes = (guint8 *)fvalue_get(&fi->value);
                printf("hijack_call %s: %s\n"
                            , finfo_abbrev
                            ,  bytes_to_str(bytes, fvalue_length(&fi->value))
                      );
                break;
            }
        case FT_ABSOLUTE_TIME:
            printf("hijack_call %s: %s\n"
                        , finfo_abbrev
                        , abs_time_to_str((const nstime_t *)fvalue_get(&fi->value)
                            , (absolute_time_display_e)fi->hfinfo->display
                            , TRUE )
                  );
            break;
        case FT_RELATIVE_TIME:
            printf("hijack_call %s: %s\n"
                        , finfo_abbrev
                        , rel_time_to_secs_str((const nstime_t *)fvalue_get(&fi->value)));
            break;
        case FT_BOOLEAN:
        case FT_UINT8:
        case FT_UINT16:
        case FT_UINT24:
        case FT_UINT32:
        case FT_FRAMENUM:
            printf("hijack_call %s: %u\n", finfo_abbrev, fvalue_get_uinteger(&fi->value));
            break;
        case FT_INT64:
        case FT_UINT64:
        case FT_EUI64:
            printf("hijack_call %s: %lu\n", finfo_abbrev, fvalue_get_integer64(&fi->value));
            break;
        case FT_INT8:
        case FT_INT16:
        case FT_INT24:
        case FT_INT32:
            printf("hijack_call %s: %d\n", finfo_abbrev, fvalue_get_sinteger(&fi->value));
            break;
        case FT_IPv4:
            {
                ipv4_addr* ipv4 = (ipv4_addr *)fvalue_get(&fi->value);
                guint32 n_addr = ipv4_get_net_order_addr(ipv4);
                printf("hijack_call %s: %s\n", finfo_abbrev, ip_to_str((guint8 *)&n_addr));
                break;
            }
        case FT_IPv6:
            {
                address            addr;
                struct e_in6_addr* ipv6 = (struct e_in6_addr *)fvalue_get(&fi->value);
                SET_ADDRESS (&addr, AT_IPv6, sizeof(struct e_in6_addr), ipv6);
                char buf[128] = {0};
                address_to_str_buf(&addr, buf, 128);
                printf("hijack_call %s: %s\n", finfo_abbrev, buf);
                break;
            }
        case FT_ETHER:
            printf("hijack_call %s: %s\n"
                        , finfo_abbrev
                        , bytes_to_str_punct((const guint8 *)fvalue_get(&fi->value), FT_ETHER_LEN, ':')
                  );
            break;
        case FT_GUID:
            printf("hijack_call %s: %s\n"
                        , finfo_abbrev
                        , guid_to_str((e_guid_t *)fvalue_get(&fi->value))
                  );
            break;
        case FT_OID:
            {
                guint8* bytes = (guint8 *)fvalue_get(&fi->value);
                printf("hijack_call %s: %s %s\n"
                            , finfo_abbrev
                            , oid_resolved_from_encoded(bytes, fvalue_length(&fi->value))
                            , oid_encoded2string(bytes, fvalue_length(&fi->value))
                      );
                break;
            }
        case FT_FLOAT:
        case FT_DOUBLE:
            printf("hijack_call %s: %g\n", finfo_abbrev, fvalue_get_floating(&fi->value));
            break;
        case FT_STRING:
        case FT_STRINGZ:
        case FT_UINT_STRING:
            {
                guint8* bytes = (guint8 *)fvalue_get(&fi->value);
                printf("hijack_call %s: %s\n", finfo_abbrev, format_text(bytes, strlen(bytes)));
                break;
            }
        case FT_IPXNET:
        case FT_PCRE:
        default:
            break;
    }
    return;
}


#define fvalue_set(i ...) do{           \
    fvalue_set(i);                      \
    hijack_call(fi);                    \
}while(0)                               \


#define fvalue_set_uinteger(i ...) do{  \
    fvalue_set_uinteger(i);             \
    hijack_call(fi);                    \
}while(0)                               \


#define fvalue_set_sinteger(i ...) do{  \
    fvalue_set_sinteger(i);             \
    hijack_call(fi);                    \
}while(0)                               \


#define fvalue_set_integer64(i ...) do{ \
    fvalue_set_integer64(i);            \
    hijack_call(fi);                    \
}while(0)                               \


#define fvalue_set_floating(i ...) do{  \
    fvalue_set_floating(i);             \
    hijack_call(fi);                    \
}while(0)                               \



