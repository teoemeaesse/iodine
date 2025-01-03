#include "platform/platform.h"

#ifdef IO_WINDOWS

#include <chrono>

#include "debug/log.h"

namespace iodine::core {
    static u32 signalState;                  ///< The current signal status.
    static std::chrono::steady_clock clock;  ///< The current clock.

    static void handleSignal(i32 signal);

    void Platform::init() {
        signal(SIGINT, handleSignal);
        signal(SIGTERM, handleSignal);
        signal(SIGABRT, handleSignal);
        signal(SIGSEGV, handleSignal);
        signal(SIGFPE, handleSignal);
        signal(SIGILL, handleSignal);
        signal(SIGHUP, handleSignal);
    }

    const char* Platform::getName() { return PLATFORM_NAME; }

    void Platform::log() { IO_INFOV("Running on {0}", getName()); }

    void Platform::assertCompatible() {
        if (!isWindows()) {
            IO_FATALV("Incompatible platform: {0}", getName());
            std::exit(EXIT_FAILURE);
        }
    }

    b8 Platform::isWindows() { return true; }

    b8 Platform::isMacOS() { return false; }

    b8 Platform::isUnixLike() { return false; }

    static void handleSignal(i32 signal) {
        switch (signal) {
            case SIGINT:
                IO_INFO("Caught signal SIGINT - raised by user on ctrl-c, alt-f4 or window close");
                signalState |= Platform::Signal::INT;
                break;
            case SIGTERM:
                IO_INFO("Caught signal SIGTERM - raised by an external process to terminate the program");
                signalState |= Platform::Signal::TERM;
                break;
            case SIGABRT:
                IO_INFO("Caught signal SIGABRT - assertion failed");
                signalState |= Platform::Signal::ABRT;
                break;
            case SIGSEGV:
                IO_INFO("Caught signal SIGSEGV - segmentation fault detected, dumping memory log");  // TODO: Implement memory tracking
                signalState |= Platform::Signal::SEGV;
                break;
            case SIGFPE:
                IO_INFO("Caught signal SIGFPE - invalid math operation");
                signalState |= Platform::Signal::FPE;
                break;
            case SIGILL:
                IO_INFO("Caught signal SIGILL - illegal instruction");
                signalState |= Platform::Signal::ILL;
                break;
            case SIGHUP:
                IO_INFO("Caught signal SIGHUP - hangup detected, reloading engine");  // TODO: Implement hot reloading (plugins etc.)
                signalState |= Platform::Signal::HUP;
                break;
            default:
                IO_WARNV("Caught unhandled signal {0}", signal);
                break;
        }
    }

    void Platform::clearSignal(Signal signal) { signalState &= ~static_cast<u32>(signal); }

    b8 Platform::isSignal(Signal signal) { return signalState & static_cast<u32>(signal); }

    u64 Platform::getTime() { return std::chrono::duration_cast<std::chrono::microseconds>(clock.now().time_since_epoch()).count(); }
}  // namespace iodine::core

#endif