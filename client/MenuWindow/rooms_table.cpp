#include "rooms_table.h"
#include <QHeaderView>
#include <QJsonValueRef>
#include <QSizePolicy>

RoomsTable::RoomsTable(QWidget *parent):
	QTableWidget(parent)
{
	setColumnCount(2);
	setRowCount(10);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSizeAdjustPolicy(QTableWidget::AdjustToContents);

	verticalHeader()->setVisible(false);
	setHorizontalHeaderLabels(QStringList({"Name", "Players"}));
	setShowGrid(true);

	connect(this, &QTableWidget::cellDoubleClicked, [this](int row, int /* column */)
	{
		emit joinRoomRequested(item(row, 0)->text());
	});
}

void RoomsTable::updateRooms(QJsonArray rooms)
{
	int rowCount = 0;
	setRowCount(rooms.size());
	for(auto room: rooms) {
		QTableWidgetItem *newRoomNameItem = new QTableWidgetItem(room.toObject()["room_name"].toString());
		newRoomNameItem->setToolTip(newRoomNameItem->text());
		setItem(rowCount, 0, newRoomNameItem);
		setItem(rowCount, 1, new QTableWidgetItem(QString::number(room.toObject()["players_count"].toInt()) + "/2"));
		rowCount++;
	}
}
