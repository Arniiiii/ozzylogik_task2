#include <fstream>

#include "fmtlog/fmtlog.h"

#include "boost/asio.hpp"
#include "boost/asio/detail/throw_error.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/detail/error_code.hpp"

#include "task2client/config.hpp"
#include "task2client/config_cmd.hpp"

using boost::asio::ip::tcp;

int main(int argc, char** argv, char** envp)
{
  task2client::Config cfg;
  int errc = setConfigViaCmd(cfg, argc, argv, envp);
  if (errc != 0)
    {
      return errc;
    }

  try
    {
      boost::asio::io_context io_context;

      tcp::socket s(io_context);
      tcp::resolver resolver(io_context);

      logi("Connecting to {} at port {} via TCP...", cfg.destination, cfg.port);
      fmtlog::poll();

      boost::asio::connect(s, resolver.resolve(cfg.destination, std::to_string(cfg.port)));

      logi("Connected.");
      logi("Openning file {} for writing and starting receiving...", cfg.response_file);
      fmtlog::poll();

      std::ofstream fout;
      fout.open(cfg.response_file);
      std::array<char, 4096> buf{};
      boost::system::error_code errc{};
      while (!errc)
        {
          size_t reply_length = boost::asio::read(s, boost::asio::buffer(buf, buf.size()), errc);
          fout.write(buf.data(), reply_length);
        }
      if (errc && errc != boost::asio::error::eof)
        {
          boost::asio::detail::throw_error(errc);
        }
      logi("Success. Closing connection and file {} .", cfg.response_file);
      fmtlog::poll();
      fout.close();
    }
  catch (std::exception& e)
    {
      loge("Exception: {}", e.what());
      fmtlog::poll();
      return 1;
    }

  return 0;
}
