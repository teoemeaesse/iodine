#pragma once

#include <string>

#include "chrono/rate.h"
#include "prelude.h"

namespace iodine::core {
    /**
     * @brief The base class for all iodine applications.
     */
    class Application {
        public:
        struct Config;

        /**
         * @brief Creates a new application.
         * @param config The configuration for the application.
         * @return The application.
         */
        explicit Application(const Config& config);
        virtual ~Application() = default;

        /**
         * @brief Initializes the application.
         */
        virtual void setup() = 0;

        /**
         * @brief Cleans up the application.
         */
        virtual void shutdown() = 0;

        /**
         * @brief Runs at a fixed time step.
         */
        virtual void tick() = 0;

        /**
         * @brief Runs as fast as possible.
         * @param delta The time since the last frame.
         */
        virtual void render(f64 delta) = 0;

        /**
         * @brief Starts or resumes the game loop.
         */
        void start();
        /**
         * @brief Pauses the game loop.
         */
        void pause();
        /**
         * @brief Stops the game loop.
         */
        void stop();
        /**
         * @brief Finishes the application.
         */
        void finish();

        /**
         * @brief Configuration for the application.
         */
        struct Config {
            // General settings.
            std::string title = "Iodine";  ///< The title of the application. Window title should default to this.
            u32 tickRate = 60;             ///< The target update rate of the application.
            u32 renderRate = 60;           ///< The target framerate of the application. 0 will sync with tick rate.

            // Metrics. Enable as needed.
            b8 isMemoryLogging = false;       ///< Whether to log memory allocations.
            b8 isPerformanceLogging = false;  ///< Whether to log performance metrics.

            // These are not too important, just leave them as they are.
            f64 tickRateWindow = 1.0f;    ///< The window to average the tick rate over (in seconds).
            f64 renderRateWindow = 1.0f;  ///< The window to average the render rate over (in seconds).
        };

        /**
         * @brief A builder for the application configuration.
         */
        class Builder {
            public:
            Builder& setTitle(const std::string& title) {
                config.title = title;
                return *this;
            }

            Builder& setTickRate(u32 tickRate) {
                config.tickRate = tickRate;
                return *this;
            }

            Builder& setTickRateWindow(f64 tickRateWindow) {
                config.tickRateWindow = tickRateWindow;
                return *this;
            }

            Builder& setRenderRate(u32 renderRate) {
                config.renderRate = renderRate;
                return *this;
            }

            Builder& setRenderRateWindow(f64 renderRateWindow) {
                config.renderRateWindow = renderRateWindow;
                return *this;
            }

            Builder& enableMemoryLogging() {
                config.isMemoryLogging = true;
                return *this;
            }

            Builder& enablePerformanceLogging() {
                config.isPerformanceLogging = true;
                return *this;
            }

            Config build() { return config; }

            private:
            Config config;
        };

        /**
         * @brief The status of the application.
         */
        enum class Status {
            Ok,     ///< The application is running.
            Error,  ///< An error occurred.
            Pause,  ///< The application is paused - render but don't tick.
            Done    ///< The application has finished.
        };

        protected:
        Config config;           ///< The configuration for the application.
        Status status;           ///< The status of the application.
        RateTracker tickRate;    ///< The rate tracker for the tick rate.
        RateTracker renderRate;  ///< The rate tracker for the render rate.

        private:
        /**
         * @brief The main game loop. Calls tick() and render() in a loop.
         */
        void loop();
    };

}  // namespace iodine::core

/**
 * @brief Creates a new application. This function must be implemented by the user.
 * @return A pointer to the application.
 */
iodine::core::Application* createApplication();