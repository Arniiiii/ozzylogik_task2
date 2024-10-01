#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <memory_resource>

#include "fmtlog/fmtlog.h"

#include "boost/asio/as_tuple.hpp"
#include "boost/asio/basic_stream_file.hpp"
#include "boost/asio/error.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/stream_file.hpp"
#include "boost/asio/write.hpp"
#include "boost/cobalt.hpp"
#include "boost/cobalt/op.hpp"
#include "boost/cobalt/thread.hpp"
#include "boost/predef.h"
#include "boost/system/detail/error_code.hpp"
#include <sys/socket.h>

#include "task2server/config.hpp"
#include "task2server/config_cmd.hpp"

template <typename T = size_t, typename U = size_t> consteval auto my_integer_pow(T base, U exp)
{
  // UB if overflow.
  // not efficient if exp is big
  // good enough in my case.
  T result = 1;
  while (exp != 0)
    {
      result *= base;
      --exp;
    }
  return result;
}

boost::cobalt::promise<void> echo(
    boost::cobalt::use_op_t::as_default_on_t<boost::asio::ip::tcp::socket> socket,
    std::string const& filename)
{
  try
    {
      auto file = boost::asio::stream_file(socket.get_executor(), filename,
                                           boost::asio::stream_file::read_only);
      std::array<char, my_integer_pow(2, 12)> data{};
      logi("Sending the file \"{}\" to ip {} at port {} by protocol {}.", filename,
           socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port(), "tcp");
      boost::system::error_code errc{};
      while (!errc && file.is_open() && socket.is_open())
        {
          fmtlog::poll();
          auto [errc_file, n] = co_await file.async_read_some(
              boost::asio::buffer(data), boost::asio::as_tuple(boost::cobalt::use_op));
          errc = errc_file;
          logd("Hello there. n = {}", n);
          co_await boost::asio::async_write(socket, boost::asio::buffer(data.data(), n));
        }
      if (errc && errc != boost::asio::error::eof)
        {
          boost::asio::detail::throw_error(errc);
        }
      logi("Success!");
      logd("Closing connection and file.");
      fmtlog::poll();
      file.close();
      socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      socket.close();
    }
  catch (std::exception& e)
    {
      std::printf("echo: exception: %s\n", e.what());
    }
}

boost::cobalt::generator<boost::asio::ip::tcp::socket> listen(uint16_t port)
{
  boost::cobalt::use_op_t::as_default_on_t<boost::asio::ip::tcp::acceptor> acceptor(
      {co_await boost::cobalt::this_coro::executor}, {boost::asio::ip::tcp::v4(), port});
  for (;;)  // <1>
    {
      boost::cobalt::use_op_t::as_default_on_t<boost::asio::ip::tcp::socket> sock
          = co_await acceptor.async_accept();  // <2>
      co_yield std::move(sock);                // <3>
    }
  co_return boost::cobalt::use_op_t::as_default_on_t<boost::asio::ip::tcp::socket>{
      acceptor.get_executor()};  // <4>
}
// end::listen[]

// tag::run_server[]
boost::cobalt::promise<void> run_server(boost::cobalt::wait_group& workers,
                                        task2server::Config const& cfg)
{
  auto l = listen(cfg.port);  // <1>
  while (true)
    {
      if (workers.size() == cfg.amount_coro_workers)
        {
          co_await workers.wait_one();  // <2>
        }
      else
        {
          workers.push_back(echo(co_await l, cfg.response_file));  // <3>
        }
    }
}
// end::run_server[]

boost::cobalt::task<int> main_func(task2server::Config const& cfg)
{
  auto configured_run_server = [&cfg = std::as_const(cfg)](boost::cobalt::wait_group& workers) {
    return run_server(workers, cfg);
  };
  co_await boost::cobalt::with(boost::cobalt::wait_group(), configured_run_server);  // <1>
  co_return 0;
}

int main(int argc, char** argv, char** envp)
{
  task2server::Config cfg;
  if (setConfigViaCmd(cfg, argc, argv, envp) != 0)
    {
      return 0;
    }
  std::ios_base::sync_with_stdio(false);
  if (cfg.threads == 1)
    {
      auto errc = run(main_func(cfg));
      return errc;
    }

  loge("Sorry, more threads than 1 is not implemented.");
  return 1;
  // std::vector<std::thread> thread_pool;
}
