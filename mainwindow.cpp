#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "iothread.h"
#include "config.h"
#include "settings.h"

static MainWindow* maininstance;

MainWindow* MainWindow::instance() {
    return maininstance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    maininstance = this;

    setTemp = 0;
    curTemp = 0;
    onOffFlag = 0;
    bathMode = 0;
    step = 8;
    memset(preset, 0, sizeof(preset));

    ui->setupUi(this);

    for (int i = 0; i < PRESETCOUNT; i++) {
        QPresetButton* p = new QPresetButton(i, QString(i+1), this);
        p->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        QFont font = p->font();
        font.setPointSize(36);
        p->setFont(font);
        presetButton[i] = p;
        ui->showerBottomPane->insertWidget(i, p, 0);
    }
    for (int i = 0; i < PRESETCOUNT; i++) {
        QPresetButton* p = new QPresetButton(PRESETCOUNT+i, QString(i+1), this);
        p->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        QFont font = p->font();
        font.setPointSize(36);
        p->setFont(font);
        presetButton[PRESETCOUNT+i] = p;
        ui->bathBottomPane->insertWidget(i, p, 0);
    }

    QMainWindow::showFullScreen();

	switch (config.settingsType) {
	case QSETTINGS: settings = new FileSettings(); break;
	case FRAM: settings = new FRAMSettings(); break;
	}
    readSettings();

    switch (config.sensorType) {
	case ONEWIRE: input = new Input_Onewire(); break;
	case I2CSENSOR: input = new Input_I2CSensor(); break;
	case ADS1115: input = new Input_ADS1115(); break;
	case LTC2451: input = new Input_LTC2451(); break;
    }

    switch (config.outputType) {
	case SERVO: output = new Output_Servo(); break;
	case MOTOR: output = new Output_Motor(); break;
	case MOTORSOFTPWM: output = new Output_Motor_SoftPWM(); break;
	case STEPPER: output = new Output_Stepper(); break;
    }

    switch (config.onOffType) {
	case DOUBLESOLENOID: onOff = new OnOff_DoubleSolenoid(); break;
    }

    if (bathMode) {
        ui->bathButton->setChecked(true);
        onOff->bath();
    } else {
        ui->showerButton->setChecked(true);
        onOff->shower();
    }

	inthread = new InThread(this);
	connect(inthread, &InThread::update, this, &MainWindow::update);
	connect(inthread, &OutThread::finished, inthread, &QObject::deleteLater);
	inthread->start();

	outthread = new OutThread(this);
	connect(outthread, &OutThread::noHotWater, this, &MainWindow::noHotWater);
	connect(outthread, &OutThread::fullhot, this, &MainWindow::fullhot);
	connect(outthread, &OutThread::fullcold, this, &MainWindow::fullcold);
	connect(outthread, &OutThread::finished, outthread, &QObject::deleteLater);
	outthread->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

MainWindow::~MainWindow() {
    cleanup();
    delete ui;
}

void MainWindow::cleanup() {
	if (outthread) {
		outthread->requestInterruption();
		outthread->wait();
		delete outthread;
		outthread = NULL;
    }
	if (inthread) {
		inthread->requestInterruption();
		inthread->wait();
		delete inthread;
		inthread = NULL;
	}
    if (onOff) {
        delete onOff;
        onOff = NULL;
    }
    if (output) {
        delete output;
        output = NULL;
    }
    if (input) {
        delete input;
        input = NULL;
    }
    writeSettings();
}

void MainWindow::readSettings() {
	setTemp = settings->value("setTemp", 405).toInt(); // Average shower temperature is supposedly 105F, which is ~40.5C
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));

    for (int i = 0; i < (PRESETCOUNT*2); i++) {
		preset[i] = settings->value("preset"+QString::number(i), 405).toInt();
		presetButton[i]->setText(asTemp(preset[i]/10.0));
    }

	bathMode = settings->value("bathMode", 0).toBool();

	config.useF = settings->value("useF", 0).toBool();
	if (config.useF) ui->useF->setChecked(true);
	else ui->useC->setChecked(true);
}
void MainWindow::writeSettings() {
	settings->cork();
	settings->setValue("setTemp", setTemp);
    for (int i = 0; i < (PRESETCOUNT*2); i++) {
		settings->setValue("preset"+QString::number(i), preset[i]);
    }
	settings->setValue("useF", config.useF);
	settings->uncork();
}

void MainWindow::loadPreset(qint32 p) {
    lock.lock();
    setTemp = preset[p];
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));
    writeSettings();
    lock.unlock();
}
void MainWindow::savePreset(qint32 p) {
    lock.lock();
    preset[p] = setTemp;
	presetButton[p]->setText(asTemp(preset[p]/10.0));
    writeSettings();
    lock.unlock();
}

qint32 MainWindow::getSetTemp() {
    qint32 ret;
    lock.lock();
    ret = setTemp;
    lock.unlock();
    return ret;
}
void MainWindow::setSetTemp(qint32 t) {
    lock.lock();
    setTemp = t;
    if (setTemp > config.maxTemp) setTemp = config.maxTemp;
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));
    tick();
    writeSettings();
    lock.unlock();
}
const QString MainWindow::asTemp(double degrees) {
	if (config.useF) {
		return QString().sprintf("%.1lf", degrees * 1.8 + 32.05); // Extra .05 because I'd rather round than truncate.
	} else {
		return QString().sprintf("%.1lf", degrees);
	}
}
const QString MainWindow::asDegrees(double degrees) {
	if (config.useF) {
		return QString().sprintf("%.1lf°F", degrees * 1.8 + 32.05);
	} else {
		return QString().sprintf("%.1lf°C", degrees);
	}
}

bool MainWindow::isOn() {
    return onOffFlag;
}
qint32 MainWindow::getTab() {
	return ui->tabWidget->currentIndex();
}

qreal MainWindow::getCurTemp() {
	qreal ret;
	ctLock.lock();
	ret = curTemp;
	ctLock.unlock();
	return ret;
}
void MainWindow::update(qreal newTemp) {
	ctLock.lock();
    curTemp = newTemp;
	ctLock.unlock();
}
void MainWindow::noHotWater() {
	onOffFlag = 0;
	ui->onOffButton->setChecked(false);
	ui->onOffButtonBath->setChecked(false);
	ui->onOffButtonManual->setChecked(false);
	ui->tabWidget->tabBar()->setEnabled(true);
}
void MainWindow::fullhot() {
    switch (ui->tabWidget->currentIndex()) {
    case 1: ui->setTemp->setStyleSheet("QLabel { color: red; }"); break;
    case 2: ui->setTempBath->setStyleSheet("QLabel { color: red; }"); break;
    }
}
void MainWindow::fullcold() {
    switch (ui->tabWidget->currentIndex()) {
    case 1: ui->setTemp->setStyleSheet("QLabel { color: blue; }"); break;
    case 2: ui->setTempBath->setStyleSheet("QLabel { color: blue; }"); break;
    }
}

void MainWindow::tick() {
    switch (ui->tabWidget->currentIndex()) {
    case 0: tickHome(); break;
    case 1: tickShower(); break;
    case 2: tickBath(); break;
    case 3: tickManual(); break;
    }
}

void MainWindow::tickHome() {
	ui->clockHome->setText(QTime::currentTime().toString(config.timeStr));
	ui->dateHome->setText(QDateTime::currentDateTime().toString(config.dateStr));
}

void MainWindow::setMode(qint32 mode) {
    switch (mode) {
    case 0: // Shower Mode
        ui->showerButton->setChecked(true);
        ui->bathButton->setChecked(false);
        bathMode = false;
        if (setTemp > config.maxTemp) setTemp = config.maxTemp;
        break;
    case 1: // Bath Mode
        ui->bathButton->setChecked(true);
        ui->showerButton->setChecked(false);
        bathMode = true;
        break;
    }
}

void MainWindow::modSetTemp(qint32 val) {
    lock.lock();
    setTemp += val;
    if (setTemp > config.maxTemp) setTemp = config.maxTemp;
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));
    writeSettings();
    lock.unlock();
}

void MainWindow::on_showerButton_clicked() {
    setMode(0);
}
void MainWindow::on_bathButton_clicked() {
    setMode(1);
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    switch (index) {
    case 0: // Home
        tickHome();
        break;
    case 1: // Shower
        setMode(0);
        tickShower();
        break;
    case 2: // Bath
        setMode(1);
        tickBath();
        break;
    case 3: // Manual
        tickManual();
    }
}
