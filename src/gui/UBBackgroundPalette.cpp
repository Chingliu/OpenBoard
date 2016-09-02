#include "UBBackgroundPalette.h"

UBBackgroundPalette::UBBackgroundPalette(QList<QAction*> actions, QWidget * parent)
    : UBActionPalette(parent)
{
    init();
    setActions(actions);
}


UBBackgroundPalette::UBBackgroundPalette(QWidget * parent)
     : UBActionPalette(parent)
{
    init();
}



void UBBackgroundPalette::init()
{
    UBActionPalette::clearLayout();
    delete layout();


    m_customCloseProcessing = false;

    mButtonSize = QSize(32, 32);
    mIsClosable = false;
    mAutoClose = false;
    mButtonGroup = 0;
    mToolButtonStyle = Qt::ToolButtonIconOnly;
    mButtons.clear();

    mVLayout = new QVBoxLayout(this);
    mTopLayout = new QHBoxLayout();
    mBottomLayout = new QHBoxLayout();

    mVLayout->addLayout(mTopLayout);
    mVLayout->addLayout(mBottomLayout);

    mSlider = new QSlider(Qt::Horizontal);

    mSlider->setMinimum(UBSettings::settings()->minCrossSize);
    mSlider->setMaximum(UBSettings::settings()->maxCrossSize);
    mSlider->setSingleStep(2);
    mSlider->setTracking(true); // valueChanged() is emitted during movement and not just upon releasing the slider

    mSliderLabel = new QLabel(tr("Grid size"));

    mBottomLayout->addSpacing(16);
    mBottomLayout->addWidget(mSliderLabel);
    mBottomLayout->addWidget(mSlider);
    mBottomLayout->addSpacing(16);

    updateLayout();
}

void UBBackgroundPalette::addAction(QAction* action)
{
    UBActionPaletteButton* button = createPaletteButton(action, this);

    mTopLayout->addWidget(button);
    mActions << action;
}

void UBBackgroundPalette::setActions(QList<QAction*> actions)
{
    mMapActionToButton.clear();

    foreach(QAction* action, actions)
    {
        addAction(action);
    }

    actionChanged();
}

void UBBackgroundPalette::updateLayout()
{
    if (mToolButtonStyle == Qt::ToolButtonIconOnly) {
        mVLayout->setContentsMargins (sLayoutContentMargin / 2  + border(), sLayoutContentMargin / 2  + border()
                , sLayoutContentMargin / 2  + border(), sLayoutContentMargin / 2  + border());
    }
    else
    {
        mVLayout->setContentsMargins (sLayoutContentMargin  + border(), sLayoutContentMargin  + border()
                , sLayoutContentMargin  + border(), sLayoutContentMargin + border());

    }
   update();
}

void UBBackgroundPalette::clearLayout()
{
    while(!mTopLayout->isEmpty()) {
        QLayoutItem* pItem = mTopLayout->itemAt(0);
        QWidget* pW = pItem->widget();
        mTopLayout->removeItem(pItem);
        delete pItem;
        mTopLayout->removeWidget(pW);
        delete pW;
    }

    delete mTopLayout;

    while(!mBottomLayout->isEmpty()) {
        QLayoutItem* pItem = mBottomLayout->itemAt(0);
        QWidget* pW = pItem->widget();
        mBottomLayout->removeItem(pItem);
        delete pItem;
        mBottomLayout->removeWidget(pW);
        delete pW;
    }

    delete mBottomLayout;

    delete mVLayout;

    mActions.clear();
    mButtons.clear();
}

void UBBackgroundPalette::showEvent(QShowEvent* event)
{
    backgroundChanged();

    mSlider->setValue(UBApplication::boardController->activeScene()->backgroundGridSize());
    connect(mSlider, SIGNAL(valueChanged(int)),
            this, SLOT(sliderValueChanged(int)));

    QWidget::showEvent(event);
}

void UBBackgroundPalette::sliderValueChanged(int value)
{
    UBApplication::boardController->activeScene()->setBackgroundGridSize(value);
}

void UBBackgroundPalette::backgroundChanged()
{
    bool dark = UBApplication::boardController->activeScene()->isDarkBackground();

    if (dark)
        mSliderLabel->setStyleSheet("QLabel { color : white; }");
    else
        mSliderLabel->setStyleSheet("QLabel { color : black; }");
}

void UBBackgroundPalette::refresh()
{
    backgroundChanged();
    mSlider->setValue(UBApplication::boardController->activeScene()->backgroundGridSize());
}
