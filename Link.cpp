#include "SC_PlugIn.h"
#include <ableton/Link.hpp>
#include <chrono>
#include <pthread.h>

static InterfaceTable *ft;

static ableton::Link *gLink = NULL;
static float gTempo = 60.0;

void* make_link_callback(void* inTempo) {
  ableton::Link link(gTempo);
  link.enable(true);
  gLink = &link;
  while(1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return (void*) NULL;
}

struct LinkStatus : public Unit {

};

extern "C" {
  void LinkStatus_Ctor(LinkStatus* unit);
  void LinkStatus_next(LinkStatus* unit, int inNumSamples);
}


void LinkStatus_Ctor(LinkStatus* unit) {
  if (!gLink) {
    pthread_t thread;
    gTempo = *IN(0);
    pthread_create(&thread, NULL, make_link_callback, (void*)NULL);
  }
  SETCALC(LinkStatus_next);
}

void LinkStatus_next(LinkStatus* unit, int inNumSamples) {

}


struct Link : public Unit {

};

extern "C" {
  void Link_Ctor(Link* unit);
  void Link_next(Link* unit, int inNumSamples);
}


void Link_Ctor(Link* unit) {
  if (!gLink) {
    Print("warn: Link not enabled!\n");
  }
  SETCALC(Link_next);
}

void Link_next(Link* unit, int inNumSamples) {
  float* output = OUT(0);
  *output = 0.0;
  if (gLink) {
    const auto time = gLink->clock().micros();
    auto timeline = gLink->captureAudioTimeline();
    const auto beats = timeline.beatAtTime(time, 4);
    *output =  beats;
  }
}


struct LinkTempo : public Unit {
  double mCurTempo;
  double mTempoCalc;
  bool mRunning;
};

extern "C" {
  void LinkTempo_Ctor(LinkTempo* unit);
  void LinkTempo_next(LinkTempo* unit, int inNumSamples);
}


void LinkTempo_Ctor(LinkTempo* unit) {
  unit->mRunning = true;

  if (!gLink) {
    Print("Link not enabled! can't set Link Tempo!\n");
    unit->mRunning = false;
  }
  
  if (gLink && unit->mRunning) {
    const auto timeline = gLink->captureAudioTimeline();
    unit->mCurTempo = timeline.tempo();
    unit->mTempoCalc = unit->mCurTempo - *IN(0);
  }
  SETCALC(LinkTempo_next);
}

void LinkTempo_next(LinkTempo* unit, int inNumSamples) {
  if (gLink && unit->mRunning) {
    auto timeline = gLink->captureAudioTimeline();
    timeline.setTempo(unit->mCurTempo - (*IN(1) * unit->mTempoCalc), gLink->clock().micros());
    gLink->commitAudioTimeline(timeline);
  }
}

PluginLoad(Link) {
  ft = inTable;
  DefineSimpleUnit(LinkStatus);
  DefineSimpleUnit(Link);
  DefineSimpleUnit(LinkTempo);
}

