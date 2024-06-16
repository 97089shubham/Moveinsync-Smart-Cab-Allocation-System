import pandas as pd
import random
import time
import csv
import matplotlib.pyplot as plt

random.seed(1)

locations = pd.read_csv('BangaloreAreaLatLongDetails.csv')

num_cabs = 10  # total no of cabs
num_stationary = 5  # no of stationary cabs
cabs = []

# initializing the cabs
for i in range(num_cabs):
    loc_index = random.randint(0, len(locations) - 1)
    is_stationary = (i < num_stationary)
    if not is_stationary:
        destination_index = random.randint(0, len(locations) - 1)
        while destination_index == loc_index:  # Ensure destination is different from source
            destination_index = random.randint(0, len(locations) - 1)
        destination = {
            'Latitude': locations.iloc[destination_index]['Latitude'],
            'Longitude': locations.iloc[destination_index]['Longitude']
        }
        source = {
            'Latitude': locations.iloc[loc_index]['Latitude'],
            'Longitude': locations.iloc[loc_index]['Longitude']
        }
    else:
        destination = None
        source = {
            'Latitude': locations.iloc[loc_index]['Latitude'],
            'Longitude': locations.iloc[loc_index]['Longitude']
        }

    cabs.append({
        'id': i + 1,
        'Latitude': locations.iloc[loc_index]['Latitude'],
        'Longitude': locations.iloc[loc_index]['Longitude'],
        'source_latitude': source['Latitude'],
        'source_longitude': source['Longitude'],
        'dest_latitude': destination['Latitude'] if destination else None,
        'dest_longitude': destination['Longitude'] if destination else None,
        'moving': not is_stationary
    })

# write all the updates of cab movement in a csv file, so that it can be tracked
def write_cab_data_to_csv(cabs):
    with open('cab_data.csv', mode='w', newline='') as file:
        fieldnames = ['id', 'Latitude', 'Longitude', 'source_latitude', 'source_longitude','dest_latitude', 'dest_longitude', 'moving']
        writer = csv.DictWriter(file, fieldnames=fieldnames)

        writer.writeheader()
        for cab in cabs:
            writer.writerow({
                'id': cab['id'],
                'Latitude': cab['Latitude'],
                'Longitude': cab['Longitude'],
                'source_latitude': cab['source_latitude'],
                'source_longitude': cab['source_longitude'],
                'dest_latitude': cab['dest_latitude'],
                'dest_longitude': cab['dest_longitude'],
                'moving': cab['moving']
            })

# plotting the cabs
def update_and_plot(frame):
    cabs_df = pd.read_csv('cab_data.csv')

    plt.clf()
    for index, cab in cabs_df.iterrows():
        color = 'red' if not cab['moving'] else 'blue'
        plt.scatter(cab['Longitude'], cab['Latitude'], c=color)
        plt.text(cab['Longitude'], cab['Latitude'], f'ID: {int(cab["id"])}', fontsize=9, ha='center', va='bottom')

    min_lat = locations['Latitude'].min()
    max_lat = locations['Latitude'].max()
    min_lon = locations['Longitude'].min()
    max_lon = locations['Longitude'].max()
    pad = 0.002  # Padding value
    plt.xlim(min_lon - pad, max_lon + pad)
    plt.ylim(min_lat - pad, max_lat + pad)

    plt.xlabel('Longitude')
    plt.ylabel('Latitude')
    plt.title('Real-time Cab Positions')
    plt.grid(True)

# reading data from a csv file in which new cabs are booked by employees
def update_cab(external_file):
    try:
        updates = pd.read_csv(external_file)
        for _, update in updates.iterrows():
            cab_id = update['id']
            for cab in cabs:
                if cab['id'] == cab_id:
                    cab['source_latitude'] = update['source_latitude']
                    cab['source_longitude'] = update['source_longitude']
                    cab['dest_latitude'] = update['dest_latitude']
                    cab['dest_longitude'] = update['dest_longitude']
                    cab['moving'] = True  # Mark cab as moving
    except Exception as e:
        print(f"Error reading {external_file}: {e}")

# Simulate real-time cab movements and update CSV continuously
while True:
    for cab in cabs:
        if cab['moving']:
            dest_lat = cab['dest_latitude']
            dest_lon = cab['dest_longitude']

            # Move cab towards destination
            if cab['Latitude'] < dest_lat:
                cab['Latitude'] += 0.0005  # Move up
            elif cab['Latitude'] > dest_lat:
                cab['Latitude'] -= 0.0005  # Move down

            if cab['Longitude'] < dest_lon:
                cab['Longitude'] += 0.0005  # Move right
            elif cab['Longitude'] > dest_lon:
                cab['Longitude'] -= 0.0005  # Move left

            # Check if cab has reached destination
            if round(cab['Latitude'], 2) == round(dest_lat, 2) and round(cab['Longitude'], 2) == round(dest_lon, 2):
                # Select a new random destination (different from current source)
                new_destination_index = random.randint(0, len(locations) - 1)
                while new_destination_index == loc_index:  # Ensure destination is different from source
                    new_destination_index = random.randint(0, len(locations) - 1)
                cab['dest_latitude'] = locations.iloc[new_destination_index]['Latitude']
                cab['dest_longitude'] = locations.iloc[new_destination_index]['Longitude']

    update_cab("external_cab_updates.csv");

    # Write updated cab data to CSV file
    write_cab_data_to_csv(cabs)

    # Update and plot Matplotlib
    update_and_plot(None)
    plt.pause(0.1)  # Adjust pause time for smoother animation
plt.show()