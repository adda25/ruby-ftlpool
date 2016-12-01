/*
 * * Copyright (c) 2016 Amedeo Setti, Matteo Ragni
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <ruby.h>
#include "ThreadPool/threadpool.hpp"

/* Interface */

VALUE ftlpool_hardware_threads(VALUE);

astp::ThreadPool* ftlpool_tp_pool_get(VALUE);
VALUE ftlpool_tp_pool_destroy(VALUE);
VALUE ftlpool_tp_pool_alloc(VALUE);
VALUE ftlpool_tp_pool_new(VALUE, VALUE);
VALUE ftlpool_tp_pool_size(VALUE);
VALUE ftlpool_tp_pool_resize(VALUE, VALUE);
VALUE ftlpool_tp_pool_wait(VALUE);
VALUE ftlpool_tp_pool_awake(VALUE);
VALUE ftlpool_tp_pool_stop(VALUE);
VALUE ftlpool_tp_pool_synchronize(VALUE);
VALUE ftlpool_tp_pool_end_synchronize(VALUE);

VALUE ftlpool_tp_pool_set_sleep_time_ns(VALUE, VALUE);
VALUE ftlpool_tp_pool_set_sleep_time_ms(VALUE, VALUE);
VALUE ftlpool_tp_pool_set_sleep_time_s(VALUE, VALUE);
VALUE ftlpool_tp_pool_sleep_time_ns(VALUE);

/**
 * Casting Helping Macros.
 *
 * Those two macros cast from object astp::ThreadPool* to
 * void* and from void* to astp::ThreadPool*.
 */
#define TP_VOID(o) reinterpret_cast<void *>(o)
#define TP_CLASS(o) reinterpret_cast<astp::ThreadPool *>(o)

/**
 * Hardware thread number.
 *
 * Asks to the library the number of Hardware Threads available
 * on the system.
 * @param self
 * @return Fixnum
 */
VALUE
ftlpool_hardware_threads(VALUE self) {
  return INT2NUM(astp::hwc());
}

/**
 * Destroyer.
 *
 * Destroys a ThreadPool object previously created
 * @param ThreadPool instance as Ruby VALUE
 * @return Ruby nil
 */
VALUE
ftlpool_tp_pool_destroy(VALUE tp) {
  delete (TP_CLASS(tp));
  return Qnil;
}

/**
 * Pointer to ThreadPool data structure.
 *
 * Returns a pointer to an existing data structure
 * that mantains the ThreadPool.
 * @param interpreter context
 * @return pointer to a data structure of ThreadPool instance
 */
astp::ThreadPool*
ftlpool_tp_pool_get(VALUE self) {
  astp::ThreadPool* tp_ptr;
  Data_Get_Struct(self, astp::ThreadPool, tp_ptr);
  return tp_ptr;
}

/**
 * Allocate ThreadPool pointer to the interpreter
 *
 * Request a new memory data struct for a new instance
 * of the class. Returns the pointer to the memory space.
 * It requires also a pointer to the destroy function, that handles
 * the distruction of the object.
 * @param Ruby context
 * @return pointer to a new or existing data structure
 */
VALUE
ftlpool_tp_pool_alloc(VALUE self) {
  return Data_Wrap_Struct(
    self,
    NULL,
    ftlpool_tp_pool_destroy,
    ruby_xmalloc(sizeof(astp::ThreadPool)));
}

/**
 * Initialize a new thread pool.
 *
 * You can create the thread pool with the default platform dependent
 * number of threads, or you can specify your desired number: at least
 * one thread must be created.
 * @param Ruby class instance
 * @param number of threads as Fixnum
 * @return Ruby Nil value
 */
VALUE
ftlpool_tp_pool_new(VALUE self, VALUE i) {
  int k = (FIXNUM_P(i) ? NUM2INT(i) : astp::hwc());
  try {
    astp::ThreadPool* tp_ptr = ftlpool_tp_pool_get(self);
    new (tp_ptr) astp::ThreadPool(k);
  } catch (std::runtime_error e) {
    rb_raise(rb_eRuntimeError, (char *)e.what());
  }
  return Qnil;
}

/**
 * Pool size.
 *
 * Returns the actual pool size
 * @param Ruby class instance
 * @return Fixnum pool size
 */
VALUE
ftlpool_tp_pool_size(VALUE self) {
  return INT2NUM(ftlpool_tp_pool_get(self)->pool_size());
}

/**
 * Resize pool.
 *
 * The pool can be resized after it was created: if the resizing operation
 * decreases the current number of threads, a number equal to the difference
 * is popped from the pool, but only when the threads have finished to
 * compute their workload. At least one thread must be kept in the pool.
 * During the resizing, the stop of the pool is blocked.
 * @param Ruby class instance
 * @param Fixnum new pool size
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_resize(VALUE self, VALUE i) {
  try {
    ftlpool_tp_pool_get(self)->resize(NUM2INT(i));
  } catch (std::runtime_error e) {
    rb_raise(rb_eRuntimeError, (char*)e.what());
  }
  return self;
}

/**
 * Puts pool in wait.
 *
 * When the tasks are inserted in the pool, you cannot know
 * when they will be completed. If you need to know when they
 * are completed, you can wait the execution of all the tasks
 * with the following method. This will wait, blocking the caller
 * thread, until all the tasks have finished to run.
 * @param Ruby class instance
 * @return Ruby class instance
 **/
VALUE
ftlpool_tp_pool_wait(VALUE self) {
  ftlpool_tp_pool_get(self)->wait();
  return self;
}

/**
 * Stops a pool.
 *
 * With this method all the threads in the pool will be stopped,
 * waiting the end of task execution, and then will be popped. So
 * at end of the stop exceution, the thread pool will have zero
 * threads. During the stop, the resize of the pool is blocked.
 * @param Ruby class instance
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_stop(VALUE self) {
  ftlpool_tp_pool_get(self)->stop();
  return self;
}

/**
 * Awake the pool.
 *
 * After that the stop method is called, the pool has zero threads, so
 * new tasks will not be executed. So you need to call the awake method:
 * the pool will be resized to the same number of threads that the pool
 * had before stopping
 * @param Ruby class instance
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_awake(VALUE self) {
  ftlpool_tp_pool_get(self)->awake();
  return self;
}

/**
 * Synchronize execution of a pool.
 *
 * Thread pool has four methods that allow the synchronization of the
 * threads in the pool when accessing some external critical part. These
 * methods acts with binary semaphore implemented in a nested class of
 * the thread pool.
 * @param Ruby class instance
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_synchronize(VALUE self) {
   ftlpool_tp_pool_get(self)->synchronize();
   return self;
}

/**
 * Ends synchronization.
 *
 * Checkout synchronize reference
 * @param Ruby class instance
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_end_synchronize(VALUE self) {
  ftlpool_tp_pool_get(self)->end_synchronize();
  return self;
}

/**
 * Set sleep time in nanosecond.
 *
 * Set sleep time in nanosecond.
 * @param Ruby class instance
 * @param time in milliseconds as Fixnum
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_set_sleep_time_ns(VALUE self, VALUE t) {
  ftlpool_tp_pool_get(self)->set_sleep_time_ns(NUM2INT(t));
  return self;
}

/**
 * Set sleep time in milliseconds.
 *
 * Set sleep time in milliseconds.
 * @param Ruby class instance
 * @param time in milliseconds as Fixnum
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_set_sleep_time_ms(VALUE self, VALUE t) {
  ftlpool_tp_pool_get(self)->set_sleep_time_ms(NUM2INT(t));
  return self;
}

/**
 * Set sleep time in seconds.
 *
 * Set sleep time in seconds.
 * @param Ruby class instance
 * @param time in milliseconds as Fixnum
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_set_sleep_time_s(VALUE self, VALUE t) {
  ftlpool_tp_pool_get(self)->set_sleep_time_s(NUM2INT(t));
  return self;
}

/**
 * Get sleep time in nanosecond.
 *
 * Get sleep time in nanosecond.
 * @param Ruby class instance
 * @return time in nanoseconds
 */
VALUE
ftlpool_tp_pool_sleep_time_ns(VALUE self) {
  return INT2NUM(ftlpool_tp_pool_get(self)->sleep_time_ns());
}

/**
 * Pushes a new block in ThreadPool.
 *
 * Getting a block and preparing the lambda to push inside
 * the ThreadPool. Now it is not working XD!
 * @param Ruby class instance
 * @return Ruby class instance
 */
VALUE
ftlpool_tp_pool_push(VALUE self) {
  if (!rb_block_given_p())
    rb_raise(rb_eArgError, "Block required!");

  VALUE block = rb_block_proc();
  ftlpool_tp_pool_get(self)->push([block](){
    rb_funcall(block, rb_intern("call"), 0);
  });
  return self;
}

extern "C" void
Init_FtlPoolCPP() {
  VALUE ftlpool = rb_define_module("FtlPool");
  VALUE tp      = rb_define_class_under(ftlpool, "ThreadPool", rb_cObject);

  /* Module Functions */
  rb_define_module_function(ftlpool, "hardware_threads", RUBY_METHOD_FUNC(ftlpool_hardware_threads), 0);
  /* ThreadPool Class Public Methods */
  rb_define_alloc_func(tp, ftlpool_tp_pool_alloc);
  rb_define_method(tp, "size", RUBY_METHOD_FUNC(ftlpool_tp_pool_size), 0);
  rb_define_method(tp, "wait", RUBY_METHOD_FUNC(ftlpool_tp_pool_wait), 0);
  rb_define_method(tp, "awake", RUBY_METHOD_FUNC(ftlpool_tp_pool_awake), 0);
  rb_define_method(tp, "stop", RUBY_METHOD_FUNC(ftlpool_tp_pool_stop), 0);
  rb_define_method(tp, "synchronize", RUBY_METHOD_FUNC(ftlpool_tp_pool_synchronize), 0);
  rb_define_method(tp, "end_synchronize", RUBY_METHOD_FUNC(ftlpool_tp_pool_end_synchronize), 0);
  rb_define_method(tp, "sleep_ns", RUBY_METHOD_FUNC(ftlpool_tp_pool_sleep_time_ns), 0);
  rb_define_method(tp, "push", RUBY_METHOD_FUNC(ftlpool_tp_pool_push), 0);
  /* ThreadPool Class Private Methods */
  rb_define_private_method(tp, "initialize", RUBY_METHOD_FUNC(ftlpool_tp_pool_new), 1);
  rb_define_method(tp, "size=", RUBY_METHOD_FUNC(ftlpool_tp_pool_resize), 1);
  rb_define_private_method(tp, "_set_sleep_time_ns", RUBY_METHOD_FUNC(ftlpool_tp_pool_set_sleep_time_ns), 0);
  rb_define_private_method(tp, "_set_sleep_time_ms", RUBY_METHOD_FUNC(ftlpool_tp_pool_set_sleep_time_ms), 0);
  rb_define_private_method(tp, "_set_sleep_time_s", RUBY_METHOD_FUNC(ftlpool_tp_pool_set_sleep_time_s), 0);

}
