void kpanic(const char* msg) {
    (void)msg;
    for (;;) { __asm__ volatile("cli; hlt"); }
}
