/*******************************************************************************
*                           epoll wrapper
*                       =====================
* File Name: epoll_wrapper.cpp
* Related files: epoll_wrapper.hpp
* #Version: V 1.0
* Writer: Kobi Medrish       
* Created: 6.11.19
* Last update: 10.11.19
*******************************************************************************/

/*============================================================================*/
/*                                  Definitions                               */
/*============================================================================*/
/*                                                      standard  directories */
/*                                                      ~~~~~~~~~~~~~~~~~~~~~ */
#include <iostream>
#include <assert.h> /* assert */ 
#include <sys/epoll.h> // epoll
#include <unistd.h>    // for close()

/*============================================================================*/
/*                                                          local directories */
/*                                                          ~~~~~~~~~~~~~~~~~ */
#include "include/epoll_wrapper.hpp"
/*============================================================================*/
namespace med
{

/*============================================================================*/
/*                               Class EPollWrapper                           */
/*============================================================================*/
/*                               ~~~~~~~~~~~~~~~~~                            */
/*                               special functions                            */
/*                               ~~~~~~~~~~~~~~~~~                            */
/*  									                             Constructor / ctor */
/*                                                         ~~~~~~~~~~~~~~~~~~ */
EPollWrapper::EPollWrapper(bool multithread_flag,
                           int create_flag,
                           int max_event):
                                         m_epoll_fd(epoll_create1(create_flag)),
                                         m_max_event(max_event),
                                         m_multithread_flag(multithread_flag)
                                         
{}
                                                                
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  									                              Destructor / dtor */
/*                                                          ~~~~~~~~~~~~~~~~~ */
EPollWrapper::~EPollWrapper()
{
   close(m_epoll_fd);

   // in case of single threaded mode resources allocated in wait() needs to be
   // deleted
   if (false == m_multithread_flag)
   {
      delete[] m_epoll_events;
   }
}

/*============================================================================*/
/*                               ~~~~~~~~~~~~~~~~                             */
/*                               member functions                             */
/*                               ~~~~~~~~~~~~~~~~                             */

//epoll_ctl is used to add, remove, or otherwise control the monitoring of
// an fd in the epoll set / interest list

/*                                                                        add */
/*                                                                        ~~~ */
void EPollWrapper::add(int file_descriptor_to_add,  uint32_t events_flag)
{
   // new epoll_event struct for file descriptor to be added is created and
   // initialised 
   struct epoll_event new_event = {};
   new_event.data.fd = file_descriptor_to_add;
   new_event.events = events_flag;

   // file descriptor is added with EPOLL_CTL_ADD flag
   epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, file_descriptor_to_add, &new_event);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                                                     remove */
/*                                                                     ~~~~~~ */
void EPollWrapper::remove(int file_descriptor_to_remove)
{
   epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, file_descriptor_to_remove, nullptr);
}  

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                                                       wait */
/*                                                                       ~~~~ */

int EPollWrapper::wait(epoll_event* epoll_events,
                       int maxevents,
                       int time_out_ms)
{
   int event_count = 0;

   // if more then one thread have access to the epoll it most provide its own
   // separate  event array (epoll_event) to store ready fd.   
   if (true == m_multithread_flag)
   {
      event_count = epoll_wait(m_epoll_fd,
                               epoll_events,
                               maxevents,
                               time_out_ms);
   }
   // single threaded mode, Class takes care of allocation of event array
   //  store ready fd
   else 
   {
      m_epoll_events = new epoll_event[m_max_event];

      event_count = epoll_wait(m_epoll_fd,
                               m_epoll_events,
                               maxevents,
                               time_out_ms);
   }
   
   if(-1 == event_count)
   {
      throw std::runtime_error("epoll_wait fail\n"); 
   }

   return (event_count);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                                                 operator[] */
/*                                                                 ~~~~~~~~~~ */
const epoll_event& EPollWrapper::operator[](const int index)
{
    return m_epoll_events[index];
}

} // namespace med
