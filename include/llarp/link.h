#ifndef LLARP_LINK_H_
#define LLARP_LINK_H_
#include <llarp/crypto.h>
#include <llarp/mem.h>
#include <llarp/msg_handler.h>
#include <llarp/obmd.h>
#include <llarp/ev.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * wire layer transport interface 
 */
struct llarp_link;

  
/**
 * wire layer transport session for point to point communication between us and another
 */
struct llarp_link_session;

struct llarp_link_session_listener {
  void *user;
  /** set by try_establish */
  struct llarp_link *link;
  /** set by try_establish */
  struct llarp_ai *ai;
  /** callback to handle result */
  void (*result)(struct llarp_link_session_listener *,
                 struct llarp_link_session *);
};

/** information for establishing an outbound session */
struct llarp_link_establish_job {
  struct llarp_ai *ai;
  uint64_t timeout;
};

struct llarp_link_session_iter {
  void *user;
  struct llarp_link *link;
  bool (*visit)(struct llarp_link_session_iter *, struct llarp_link_session *);
};

struct llarp_link_ev_listener
{
  void * user;
  void (*established)(struct llarp_ev_listener *, struct llarp_link_session *, bool);
  void (*timeout)(struct llarp_ev_listener *, struct llarp_link_session *, bool);
  void (*tx)(struct llarp_ev_listener *, struct llarp_link_session *, size_t);
  void (*rx)(struct llarp_ev_listener *, struct llarp_link_session *, size_t);
  void (*error)(struct llarp_ev_listener *, struct llarp_link_session *, const char *);
};

struct llarp_link
{
  void * impl;
  const char * (*name)(struct llarp_link *);
  int (*register_listener)(struct llarp_link *, struct llarp_link_ev_listener);
  void (*deregister_listener)(struct llarp_link *, int);
  bool (*configure)(struct llarp_link *, const char *, int, uint16_t);
  bool (*start_link)(struct llarp_link *, struct llarp_ev_loop *);
  bool (*stop_link)(struct llarp_link *);
  bool (*put_ai)(struct llarp_link *, struct llarp_ai *);
  void (*iter_sessions)(struct llarp_link *, struct llarp_link_session_iter);
  void (*try_establish)(struct llarp_link *,
                        struct llarp_link_establish_job,
                        struct llarp_link_session_listener);
  void (*free)(struct llarp_link *);
};

struct llarp_link_session
{
  void * impl;
  struct llarp_rc * (*remote_rc)(struct llarp_link_session *);
  /** send an entire message, splits up into smaller pieces and does encryption */
  ssize_t (*sendto)(struct llarp_link_session *, llarp_buffer_t);
  /** return true if this session is timed out */
  bool (*timeout)(struct llarp_link_session *);
  /** explicit close session */
  void (*close)(struct llarp_link_session *);
};

  
#ifdef __cplusplus
}
#endif

#endif