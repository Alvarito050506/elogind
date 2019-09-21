/* SPDX-License-Identifier: LGPL-2.1+ */

//#include <stdio.h>

//#include "format-util.h"
//#include "memory-util.h"

char *format_ifname(int ifindex, char buf[static IF_NAMESIZE + 1]) {
        /* Buffer is always cleared */
        memzero(buf, IF_NAMESIZE + 1);
        return if_indextoname(ifindex, buf);
}

char *format_bytes_full(char *buf, size_t l, uint64_t t, FormatBytesFlag flag) {
        typedef struct {
                const char *suffix;
                uint64_t factor;
        } suffix_table;
        static const suffix_table table_iec[] = {
                { "E", UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024) },
                { "P", UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024) },
                { "T", UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024) },
                { "G", UINT64_C(1024)*UINT64_C(1024)*UINT64_C(1024) },
                { "M", UINT64_C(1024)*UINT64_C(1024) },
                { "K", UINT64_C(1024) },
        }, table_non_iec[] = {
                { "E", UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000) },
                { "P", UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000) },
                { "T", UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000) },
                { "G", UINT64_C(1000)*UINT64_C(1000)*UINT64_C(1000) },
                { "M", UINT64_C(1000)*UINT64_C(1000) },
                { "K", UINT64_C(1000) },
        };
        const suffix_table *table;
        size_t i;

        assert_cc(ELEMENTSOF(table_iec) == ELEMENTSOF(table_non_iec));

        if (t == (uint64_t) -1)
                return NULL;

        table = flag & FORMAT_BYTES_USE_IEC ? table_iec : table_non_iec;

        for (i = 0; i < ELEMENTSOF(table_iec); i++)
                if (t >= table[i].factor) {
                        if (flag & FORMAT_BYTES_BELOW_POINT)
                                snprintf(buf, l,
                                         "%" PRIu64 ".%" PRIu64 "%s",
                                         t / table[i].factor,
                                         ((t*UINT64_C(10)) / table[i].factor) % UINT64_C(10),
                                         table[i].suffix);
                        else
                                snprintf(buf, l,
                                         "%" PRIu64 "%s",
                                         t / table[i].factor,
                                         table[i].suffix);

                        goto finish;
                }

        snprintf(buf, l, "%" PRIu64 "%s", t, flag & FORMAT_BYTES_TRAILING_B ? "B" : "");

finish:
        buf[l-1] = 0;
        return buf;

}
