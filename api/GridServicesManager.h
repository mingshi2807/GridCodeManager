#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class GridServiceManager
{
private:

  std::vector<std::function<void()>> services;

public:

  GridServiceManager(
      const std::initializer_list<std::function<void()>> & serviceList)
      : services(serviceList)
  {}

  void
  executeServices()
  {
    for (const auto & service : services) {
      service(); // Invoking the callable ex. lambda function
    }
  }
};

class GridServiceManagerFactory
{
public:

  static GridServiceManager
  create()
  {
    return GridServiceManager(
        {[]()
         {
           std::cout << "Concrete Grid Code Service: Performing grid code "
                        "service dispatch."
                     << '\n';
           // Grid Code Service logic...
         },
         []()
         {
           std::cout
               << "Concrete Measurement Interface: Performing measurement."
               << '\n';
           // Measurement Service logic...
         },
         []()
         {
           std::cout << "Concrete Grid Code Manager: Managing grid code."
                     << '\n';
           // Grid Code Management logic...
         },
         []()
         {
           std::cout
               << "Concrete Energy Service Manager: Managing energy service."
               << '\n';
           // Energy Service Management logic...
         }});
  }
};

/* int */
/* main() */
/* { */
/*   GridServiceManager manager = GridServiceManagerFactory::create(); */
/*   manager.executeServices(); */
/*   return 0; */
/* } */
