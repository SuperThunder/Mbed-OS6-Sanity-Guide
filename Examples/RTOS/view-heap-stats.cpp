/* in mbed_app.json,
Set platform.heap-stats-enabled to 1

See: https://os.mbed.com/docs/mbed-os/v6.15/apis/platform-options-and-config.html
*/

/*Effects of enabling heap statistics (see https://os.mbed.com/docs/mbed-os/v6.15/apis/runtime-memory-statistics.html):
- 8 extra bytes used for each memory allocation
- realloc never reuses buffer it is resizing
- memory allocation slightly slower
*/

void view_heap_stats()
{
    mbed_stats_heap_t st;
    mbed_stats_heap_get(&st);
    printf("\nStats:\nCurrent size: %d\nMax size: %d\nTotal size: %d\nReserved size: %d\nAlloc count: %d\nAlloc fail count: %d\nOverhead size: %d\n", \
            st.current_size, st.max_size, st.total_size, st.reserved_size, st.alloc_cnt, st.alloc_fail_cnt, st.overhead_size);
}