
LinkTempo : UGen {
	*kr { arg tempo, env;
		^this.multiNew('control', tempo,env);
	}
}

LinkStatus : UGen {
	*kr { arg tempo = 60.0;
		^this.multiNew('control', tempo);
	}
}



Link : UGen {
	*enable { arg tempo = 60.0;
		play {
			LinkStatus.kr(tempo);
			FreeSelf.kr(1);
		}
	}

	*kr {
		^this.multiNew('control');
	}

	*quantum {
		^96;
	}

	*setTempo { arg tempo, lagTime = 0.0;
		play{
			LinkTempo.kr(tempo, Line.kr(0, 1, lagTime, doneAction: 2));
		};
	}
}

LinkCount : UGen {
	*kr { arg divistion = 1;
		var count = Link.kr;
		var quantum = Link.quantum;
		var ratio = 1.0 / quantum;
		count = count / ratio;
		^(count / (quantum / divistion)).floor;
	}
}


LinkTrig : UGen {
	*kr { arg division = 1;
		var count = LinkCount.kr(division);
		^Changed.kr(count - Latch.kr(count,1));
	}
}


LinkLane : UGen {
	*kr { arg div = 1, max = 4, lane = [];
		^Mix(lane.collect({|item|
			var isEq = (LinkCount.kr(div) % max eq: item);
			Changed.kr(isEq) * isEq;
		}));
	}
}
