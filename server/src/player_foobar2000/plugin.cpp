#include "plugin.hpp"
#include "../project_info.hpp"
#include "utils.hpp"

#include <memory>

namespace msrv {
namespace player_foobar2000 {

Plugin::Plugin()
    : player_(),
      host_(&player_)
{
    current_ = this;
    applySettings();
}

Plugin::~Plugin()
{
    current_ = nullptr;
}

Plugin* Plugin::current_ = nullptr;

namespace {

class InitQuit : public initquit
{
public:
    void on_init() override
    {
        Logger::setCurrent(&logger_);
        tryCatchLog([this] { plugin_ = std::make_unique<Plugin>(); });
    }

    void on_quit() override
    {
        tryCatchLog([this] { plugin_.reset(); });
        Logger::setCurrent(nullptr);
    }

private:
    Fb2kLogger logger_;
    std::unique_ptr<Plugin> plugin_;
};

initquit_factory_t<InitQuit> InitQuitFactory;

DECLARE_COMPONENT_VERSION(
    MSRV_PROJECT_NAME,
    MSRV_VERSION_STRING,
    MSRV_PROJECT_DESC "\n\n" MSRV_PROJECT_URL "\n\n" MSRV_LICENSE_TEXT);

VALIDATE_COMPONENT_FILENAME(MSRV_FOOBAR2000_FILE);

}

}}
