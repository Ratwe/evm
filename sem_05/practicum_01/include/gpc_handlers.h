/*
 * gpc_handlers.h
 *
 * host and sw_kernel library
 *
 * Macro instantiation for handlers
 *
 */
#ifndef DEF_HANDLERS_H_
#define DEF_HANDLERS_H_
#define DECLARE_EVENT_HANDLER(handler) \
            const unsigned int event_ ## handler =__LINE__; \
            void handler ();
#define __event__(handler) event_ ## handler
//  Event handlers declarations by declaration line number 
DECLARE_EVENT_HANDLER(update);
DECLARE_EVENT_HANDLER(select);
DECLARE_EVENT_HANDLER(update_obj_x);
DECLARE_EVENT_HANDLER(select_obj_x);
DECLARE_EVENT_HANDLER(update_obj_y);
DECLARE_EVENT_HANDLER(select_obj_xy);
#endif
