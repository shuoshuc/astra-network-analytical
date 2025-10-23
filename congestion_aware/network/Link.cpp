/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "congestion_aware/Link.h"
#include "common/NetworkFunction.h"
#include "common/Flags.h"
#include "congestion_aware/Chunk.h"
#include "congestion_aware/Device.h"
#include <cassert>
#include <sstream>

using namespace NetworkAnalytical;
using namespace NetworkAnalyticalCongestionAware;

// declaring static event_queue
std::shared_ptr<EventQueue> Link::event_queue;

inline std::string route_to_string(const NetworkAnalyticalCongestionAware::Route& route) {
    std::ostringstream oss;
    oss << "[Link] Route: ";
    bool first = true;
    for (const auto& dev : route) {
        if (!first) {
            oss << " -> ";
        }
        first = false;
        oss << (dev ? dev->get_id() : -1);
    }
    return oss.str();
}


// declaring static event_queue
std::shared_ptr<EventQueue> Link::event_queue;

inline std::string route_to_string(const NetworkAnalyticalCongestionAware::Route& route) {
    std::ostringstream oss;
    oss << "[Link] Route: ";
    bool first = true;
    for (const auto& dev : route) {
        if (!first) {
            oss << " -> ";
        }
        first = false;
        oss << (dev ? dev->get_id() : -1);
    }
    return oss.str();
}


void Link::link_become_free(void* const link_ptr) noexcept {
    assert(link_ptr != nullptr);

    // cast to Link*
    auto* const link = static_cast<Link*>(link_ptr);

<<<<<<< HEAD
<<<<<<< HEAD
    debug_log("[Link] Link becomes free at time " +
              std::to_string(Link::event_queue->get_current_time()));
=======
    // MT: debug print
    std::cout << "[Link] Link becomes free at time " 
              << Link::event_queue->get_current_time()
              << std::endl;
>>>>>>> 8c85cdc (add more print)
=======
    debug_log("[Link] Link becomes free at time " +
              std::to_string(Link::event_queue->get_current_time()));
>>>>>>> 2b69128 (clean up logging and remove unidirectional)

    // set link free
    link->set_free();

    // process pending chunks if one exist
    if (link->pending_chunk_exists()) {
        link->process_pending_transmission();
    }
}

void Link::set_event_queue(std::shared_ptr<EventQueue> event_queue_ptr) noexcept {
    assert(event_queue_ptr != nullptr);

    // set the event queue
    Link::event_queue = std::move(event_queue_ptr);
}

Link::Link(const Bandwidth bandwidth, const Latency latency) noexcept
    : bandwidth(bandwidth),
      latency(latency),
      pending_chunks(),
      busy(false) {
    assert(bandwidth > 0);
    assert(latency >= 0);

    // convert bandwidth from GB/s to B/ns
    bandwidth_Bpns = bw_GBps_to_Bpns(bandwidth);
}

void Link::send(std::unique_ptr<Chunk> chunk) noexcept {
    assert(chunk != nullptr);

    if (busy) {
        // link is busy, add to pending chunks
        pending_chunks.push_back(std::move(chunk));
    } else {
        // service this chunk immediately
        schedule_chunk_transmission(std::move(chunk));
    }
}

void Link::process_pending_transmission() noexcept {
    // pending chunk should exist
    assert(pending_chunk_exists());

    // get chunk to process
    auto chunk = std::move(pending_chunks.front());
    pending_chunks.pop_front();

    // service this chunk
    schedule_chunk_transmission(std::move(chunk));
}

bool Link::pending_chunk_exists() const noexcept {
    // check pending chunks is not empty
    return !pending_chunks.empty();
}

void Link::set_busy() noexcept {
    // set busy to true
    busy = true;
}

void Link::set_free() noexcept {
    // set busy to false
    busy = false;
}

EventTime Link::serialization_delay(const ChunkSize chunk_size) const noexcept {
    assert(chunk_size > 0);

    // calculate serialization delay
    const auto delay = static_cast<Bandwidth>(chunk_size) / bandwidth_Bpns;

    // return serialization delay in EventTime type
    return static_cast<EventTime>(delay);
}

EventTime Link::communication_delay(const ChunkSize chunk_size) const noexcept {
    assert(chunk_size > 0);

    // calculate communication delay
    const auto delay = latency + (static_cast<Bandwidth>(chunk_size) / bandwidth_Bpns);

    // return communication delay in EventTime type
    return static_cast<EventTime>(delay);
}

void Link::schedule_chunk_transmission(std::unique_ptr<Chunk> chunk) noexcept {
    assert(chunk != nullptr);

    // link should be free
    assert(!busy);

    // set link busy
    set_busy();

    // get metadata
    const auto chunk_size = chunk->get_size();
    const auto current_time = Link::event_queue->get_current_time();

<<<<<<< HEAD
<<<<<<< HEAD
    auto src_dev = chunk->current_device();
    auto next_dev = chunk->next_device();

    std::ostringstream oss;
    oss << "[Link] Scheduling chunk transmission: "
        << "ChunkPtr=" << chunk.get()
        << ", ChunkSize=" << chunk_size
        << ", From Device=" << (src_dev ? src_dev->get_id() : -1)
        << ", To Device=" << (next_dev ? next_dev->get_id() : -1)
        << ", Time=" << current_time;
    debug_log(oss.str());
    debug_log(route_to_string(chunk->get_route()));
=======
    // MT: debug print
=======
>>>>>>> 2b69128 (clean up logging and remove unidirectional)
    auto src_dev = chunk->current_device();
    auto next_dev = chunk->next_device();

<<<<<<< HEAD
>>>>>>> 8c85cdc (add more print)
=======
    std::ostringstream oss;
    oss << "[Link] Scheduling chunk transmission: "
        << "ChunkPtr=" << chunk.get()
        << ", ChunkSize=" << chunk_size
        << ", From Device=" << (src_dev ? src_dev->get_id() : -1)
        << ", To Device=" << (next_dev ? next_dev->get_id() : -1)
        << ", Time=" << current_time;
    debug_log(oss.str());
    debug_log(route_to_string(chunk->get_route()));
>>>>>>> 2b69128 (clean up logging and remove unidirectional)

    // schedule chunk arrival event
    const auto communication_time = communication_delay(chunk_size);
    const auto chunk_arrival_time = current_time + communication_time;
    auto* const chunk_ptr = static_cast<void*>(chunk.release());
    Link::event_queue->schedule_event(chunk_arrival_time, Chunk::chunk_arrived_next_device, chunk_ptr);

    // schedule link free time
    const auto serialization_time = serialization_delay(chunk_size);
    const auto link_free_time = current_time + serialization_time;
    auto* const link_ptr = static_cast<void*>(this);
    Link::event_queue->schedule_event(link_free_time, link_become_free, link_ptr);
}
