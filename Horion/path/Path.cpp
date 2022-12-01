#include "Path.h"
#include "../DrawUtils.h"
#include "../../Utils/Logger.h"
#include "../../Utils/TimerUtil.h"
#include "../Module/ModuleManager.h"

Path::Path(const std::vector<PathSegment>& segments, bool isIncomplete) : segments(segments), isIncomplete(isIncomplete) {
}
Path::Path() : segments(), isIncomplete(false) {
}
void Path::draw(int highlight) const {
	DrawUtils::setColor(13 / 255.f, 29 / 255.f, 48 / 255.f, 1);
	bool changedColor = false;
	for (int i = 0; i < segments.size(); i++) {
		auto seg = segments[i];
		int curIndex = -i * 75;
		if (i == highlight) {
			DrawUtils::setColor(39 / 255.f, 87 / 255.f, 144 / 255.f, 1);
			changedColor = true;
		}
		else if (changedColor)
			DrawUtils::setColor(13 / 255.f, 29 / 255.f, 48 / 255.f, 1);
		seg.draw();
	}
}
bool Path::isIncomplete1() const {
	return isIncomplete;
}
void Path::cutoff(float percentageKeep) {
	if (this->getNumSegments() == 0)
		return;
	int numKeep = (int)ceilf(this->getNumSegments() * percentageKeep);
	this->segments.erase(this->segments.begin() + numKeep, this->segments.end());
}
void Path::initPathSegments() {
	// Init segments
	for (auto& seg : segments) {
		seg.init();
	}

	// Check whether we can sprint or not
	if (segments.size() > 1) {

		for (int i = 0; i < segments.size() - 2; i++) {
			auto& curSeg = segments.at(i);
			auto& nextSeg = segments.at(i + 1);
			if (curSeg.getSegmentType() != PathSegmentType::WALK || nextSeg.getSegmentType() != PathSegmentType::WALK) {
				curSeg.setAllowSprint(true);
				continue;
			}
			if (curSeg.getStart().y != curSeg.getEnd().y) {
				curSeg.setAllowSprint(true);
				continue;
			}

			auto currentTangent = curSeg.getEnd().sub(curSeg.getStart()).toFloatVector().normalize();
			auto nextTangent = nextSeg.getEnd().sub(nextSeg.getStart()).toFloatVector().normalize();
			if (currentTangent.dot(nextTangent) > 0.3f) { // make sure we're running in a similar direction
				curSeg.setAllowSprint(true);
			}
		}
	}

	this->isInitialized = true;
}
bool Path::isInitialized1() const {
	return isInitialized;
}