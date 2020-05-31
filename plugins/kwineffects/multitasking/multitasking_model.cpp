/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     Lei Su <axylp@qq.com>
 *
 * Maintainer: Lei Su <axylp@qq.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "multitasking_model.h"

DesktopThumbnailItem::DesktopThumbnailItem()
{

}

MultitaskingModel::MultitaskingModel(QObject *parent)
    : QAbstractListModel(parent)
	, m_currentIndex(1)
{
}

MultitaskingModel::~MultitaskingModel()
{
}

int MultitaskingModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_desktopThumbnailItemList.count();
}

QVariant MultitaskingModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_desktopThumbnailItemList.count()) {
        return QVariant();
    }

    const DesktopThumbnailItem &data = m_desktopThumbnailItemList[row];
    switch (role) {
    case ThumbnailRole:
//        return data.thumbnail();
    default:
        break;
    }

    return QVariant();
}



QHash<int, QByteArray> MultitaskingModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ThumbnailRole] = "dmThumbnail";
    return roles;
}

void MultitaskingModel::setWindows(int screen, int desktop, const QVariantList &windows)
{
    m_windows[screen][desktop] = windows;
}

QVariantList MultitaskingModel::windows(int screen, int desktop) const 
{
    return m_windows[screen][desktop];
}


int MultitaskingModel::numScreens() const
{
	return effects->numScreens();
}

QRect MultitaskingModel::screenGeometry(int screen) const
{
	QDesktopWidget desktop;
	return desktop.screenGeometry(screen);
}


void MultitaskingModel::setCurrentIndex(int index)
{
    if (m_currentIndex != index) {
        m_currentIndex = index;
        emit currentIndexChanged(m_currentIndex);
        emit currentDesktopChanged(m_currentIndex + 1);
    }
}

int MultitaskingModel::currentIndex() const
{
	return m_currentIndex;
}

void MultitaskingModel::load(int desktopCount)
{
	clear();
	int index = m_desktopThumbnailItemList.count();
	for (int i = 0; i < desktopCount; ++i) { 
		DesktopThumbnailItem data;
		emit beginInsertRows(QModelIndex(), index, index);
		m_desktopThumbnailItemList.append(data);
		emit endInsertRows();
	}
	emit countChanged(m_desktopThumbnailItemList.count());
}

void MultitaskingModel::append()
{
	int index = m_desktopThumbnailItemList.count();
	if(index < 0 || index >= 4) {
        return;
    }

	DesktopThumbnailItem data;
    emit beginInsertRows(QModelIndex(), index, index);
    m_desktopThumbnailItemList.append(data);
    emit endInsertRows();
	emit appendDesktop();
    emit countChanged(m_desktopThumbnailItemList.count());
}

void MultitaskingModel::remove(int index)
{
    if(index < 0 || index >= m_desktopThumbnailItemList.count()) {
        return;
    }

    if (m_desktopThumbnailItemList.count() == 1)
    {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_desktopThumbnailItemList.removeAt(index);
    endRemoveRows();
	emit removeDesktop(index + 1);
    emit countChanged(m_desktopThumbnailItemList.count());
}

void MultitaskingModel::clear()
{
	emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
	m_desktopThumbnailItemList.clear();
	emit endRemoveRows();
	emit countChanged(0);
}

int MultitaskingModel::count() const
{
    return rowCount(QModelIndex());
}
