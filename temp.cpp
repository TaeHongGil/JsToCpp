//
//  CSlot346PiggyAdventurePyramidReelWrapper.cpp
//  DoubleUCasino
//
//  Created by dg-2102-pn-002 on 2022/01/07.
//

#include "CSlot346PiggyAdventurePyramidReelWrapper.h"

#include "ArrayUtils::h"
#include "CIconContainer.h"
#include "CSlotIconSpine.h"
#include "CSpineContainer.h"
#include "define.h"

#define CMiscUtils CMiscFunc
#define getRandomIntInclusive GetRandomIntBetween

using namespace std;

CSlot346PiggyAdventurePyramidReelWrapper* CSlot346PiggyAdventurePyramidReelWrapper::create(CReelFrame* instance, Options options, SlotInfo slotInfo)
{
    CSlot346PiggyAdventurePyramidReelWrapper* item = new CSlot346PiggyAdventurePyramidReelWrapper();
    if (!item->constructor(instance, options, slotInfo)) {
        CC_SAFE_DELETE(item);
        return nullptr;
    }
    item->autorelease();
    return item;
}

bool CSlot346PiggyAdventurePyramidReelWrapper::constructor(CReelFrame* instance, Options options, SlotInfo slotInfo)
{
    _slot = slotInfo->slot;
    return CSlotReelWrapper::constructor(instance, options);
    _findIndexFunc = options.findIndexFunc;
}

CSlot346PiggyAdventurePyramidReelWrapper::CSlot346PiggyAdventurePyramidReelWrapper()
{
}

CSlot346PiggyAdventurePyramidReelWrapper::~CSlot346PiggyAdventurePyramidReelWrapper()
{
}
void CSlot346PiggyAdventurePyramidReelWrapper::_onStopSpin(STOP_SPEED stopSeed)
{
    DLOG("StopSpin Check %d", _reelIndex);
    clearAction();

    int _shiftCount = ceil(abs((int)(_reel_inner->getPositionY()) / _iconHeight));
    if (_shiftCount > 0) {
        _shiftResultIcon(_shiftCount);
    }
    _releaseBeforeIcons();

    _cursorIndex = _findIndexFunc();
    int size = (int)_playLines.size();
    for (int rowIndex = 0; rowIndex < size; ++rowIndex) {
        auto _iconId = _playLines.at(rowIndex);
        auto icon = _iconContainer->borrow(_iconId);
        icon->setPositionX(0);
        icon->_reelIndex = _reelIndex;
        icon->_rowIndex = rowIndex;
        _reel_inner->addChild(icon);
        icon->setVisible(true);
        //_resultIcons.erase(_resultIcons.begin() + rowIndex);
        if (_iconId > 10000) {
            _icon->setOpacity(255 * 0.3);
            _icon->setAnimation("dim");
        }
        auto _slot = getSlot();
        _slot.setIconTextForPyramid(_icon, _iconId, false, true);

        _resultIcons.insert(_resultIcons.begin() + rowIndex, icon);
    }
    _refineResultIconPosition();

    _deployBeforeResultIcons(_spinStartPhaseIconCount, true);

    if (_onBeforeStopSpin) {
        _onBeforeStopSpin();
    }

    _bBoundStopSpin = false;
    auto stopTime = _getStopTime(stopSeed);
    if (stopTime == STOP_SPEED::SKIP) {
        _reel_inner->setPositionY(_iconHeight - 1);
    }
    _reel_inner->setPositionY((_iconHeight - (abs((int)_reel_inner->getPositionY() % _iconHeight)) + (size * _iconHeight) - _iconHeight));

    CCCallFunc* complete = CCCallFunc::create([this]() {
        _onCompleteStopSpin();
    });
    CCActionInterval* ease = nullptr;
    if (_spinEndEaseConfig == 0) {
        ease = CCMoveTo::create(stopTime, ccp(0, 0));
    }
    else {
        ease = CCEaseBackOut::create(CCMoveTo::create(stopTime, ccp(0, 0)), _spinEndEaseConfig);
    }
    setAction(STATE::SPIN_BOUNCE, CCSequence::create(ease, complete, NULL));
    schedule(schedule_selector(CSlot346PiggyAdventurePyramidReelWrapper::_onUpdateStopSpin));
    if (_onAfterStopSpin) {
        _onAfterStopSpin();
    }
}

void CSlot346PiggyAdventurePyramidReelWrapper::_deployBeforeResultIcons(int count, bool bStart /*= false*/)
{
    count = count > 0 ? count : _spinPhaseIconCount;

    int reelLength = static_cast<int>(_reelArray.size());
    for (int rowIndex = 0; rowIndex < count; ++rowIndex) {
        int cursorIndex = (_cursorIndex - count + rowIndex + reelLength) % reelLength;
        int _iconId = _reelArray.at(cursorIndex);

        auto icon = _createBeforeResultIcon(_iconId, bStart);
        icon->setPosition(0, (_iconHeight * (_reelWindowHeight + (count - rowIndex) - 1)) - (startPositionOffset * _iconHeight));
        icon->_reelIndex = _reelIndex;
        _reel_inner->addChild(icon);
        icon->setVisible(true);
        if (_iconId > 10000) {
            _icon->setOpacity(255 * 0.3);
            _icon->setAnimation("dim");
        }
        auto _slot = getSlot();
        _slot.setIconTextForPyramid(_icon, _iconId, true, true);

        _beforeIcons.push_back(icon);
    }
}
void CSlot346PiggyAdventurePyramidReelWrapper::_deployResultIcons()
{
    bool _useFakeReel = (_fakeReelSetIndex >= 0 && _fakeReelSet.size() > _fakeReelSetIndex);
    int reelLength = static_cast<int>(_reelArray.size());
    for (int rowIndex = 0; rowIndex < _reelIconMax; ++rowIndex) {
        auto _currentIndex = (_cursorIndex + rowIndex) % reelLength;
        auto _iconId = _reelArray[_currentIndex];

        if (_useFakeReel && rowIndex < _fakeReelSet[_fakeReelSetIndex].size()) {
            _iconId = _fakeReelSet[_fakeReelSetIndex].at(rowIndex);
        }

        auto icon = _iconContainer->borrow(_iconId);
        icon->setPosition(0, _iconHeight * (_reelWindowHeight - rowIndex - 1) - (startPositionOffset * _iconHeight));
        icon->_reelIndex = _reelIndex;
        icon->_rowIndex = rowIndex;
        _reel_inner->addChild(icon);
        icon->setVisible(true);
        if (_iconId > 10000) {
            _icon->setOpacity(255 * 0.3);
            _icon->setAnimation("dim");
        }

        _resultIcons.push_back(icon);
    }
}

void CSlot346PiggyAdventurePyramidReelWrapper::replaceResultIconById(int rowIndex, int iconId)
{
    auto _oldIcon = getResultIcon(rowIndex);
    auto _newIcon = _iconContainer->borrow(iconId);

    _newIcon->setPosition(_oldIcon->getPosition());
    int reelIndex = _oldIcon->_reelIndex;

    _releaseIcon(_oldIcon);
    _reel_inner->addChild(_newIcon);
    _newIcon->setVisible(true);

    _resultIcons[rowIndex] = _newIcon;
    _newIcon->_reelIndex = reelIndex;
    _newIcon->_rowIndex = rowIndex;
    if (iconId > 10000) {
        _newIcon->setOpacity(255 * 0.3);
        _newIcon->setAnimation("dim");
    }
    auto _slot = getSlot();
    _slot.setIconTextForPyramid(_newIcon, iconId, false, true);
}
