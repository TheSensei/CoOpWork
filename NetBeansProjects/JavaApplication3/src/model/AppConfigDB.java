package model;

import java.util.ArrayList;
import java.util.Properties;

import java.util.Observer;
import java.util.Observable;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import model.DB;

public class AppConfigDB extends DB implements Observer {
    private AppConfig watchingAppConfig = null;

    public void update(Observable o, Object arg) {
        if (o.getClass().equals(AppConfig.class)) {
            AppConfig appConfig = (AppConfig) o;

            if (appConfig == watchingAppConfig)
                storeAppConfig(appConfig);
        }
    }

    // note: implicitly unwatches an already watched AppConfig
    public void watchAppConfig(AppConfig appConfig) {
        // if we're already watching an AppConfig, unwatch it
        if (watchingAppConfig != null)
            unwatchAppConfig(watchingAppConfig);

        watchingAppConfig = appConfig;
        appConfig.addObserver(this);
    }

    public void unwatchAppConfig(AppConfig appConfig) {
        watchingAppConfig = null;
        appConfig.deleteObserver(this);
    }

    public AppConfig loadAppConfig() {
        String path = basePath + "/app/general";

        Properties properties = new Properties();

        try {
            File file = new File(path);

            if (!file.exists())
                return null;

            FileInputStream stream = new FileInputStream(file);
            properties.load(stream);
            stream.close();
        }
        catch (Exception e) {
            throw new RuntimeException("load app config failed");
        }

        AppConfig appConfig = new AppConfig();

        if (properties.get("screen_refresh_interval") == null
                || properties.get("reader_read_interval") == null
                || properties.get("cross_read_enabled") == null
                || properties.get("reader_ip_address") == null)
            throw new RuntimeException("app config missing field");

        try {
            int screenRefreshInterval = new Integer(properties.get("screen_refresh_interval").toString());
            int readerReadInterval = new Integer(properties.get("reader_read_interval").toString());
            boolean xReadEnabled = new Boolean(properties.get("cross_read_enabled").toString());
            String readerIpAddress = properties.get("reader_ip_address").toString();

            appConfig.setScreenRefreshInterval(screenRefreshInterval);
            appConfig.setReaderReadInterval(readerReadInterval);
            appConfig.setXReadEnabled(xReadEnabled);
            appConfig.setReaderIpAddress(readerIpAddress);
        }
        catch (Exception e) {
            throw new RuntimeException("app config invalid value");
        }

        watchAppConfig(appConfig);

        return appConfig;
    }

    public void storeAppConfig(AppConfig appConfig) {
        String path = basePath + "/app/general";

        File parent = new File(path).getParentFile();

        if (!parent.exists()) {
            boolean status = parent.mkdirs();

            if (!status)
                throw new RuntimeException("store app config failed");
        }

        Properties properties = new Properties();

        properties.put("screen_refresh_interval", Integer.toString(appConfig.getScreenRefreshInterval()));
        properties.put("reader_read_interval", Integer.toString(appConfig.getReaderReadInterval()));
        properties.put("cross_read_enabled", Boolean.toString(appConfig.getXReadEnabled()));
        properties.put("reader_ip_address", appConfig.getReaderIpAddress());

        try {
            FileOutputStream stream = new FileOutputStream(new File(path));
            properties.store(stream, "App Configuration");
            stream.close();
        }
        catch (Exception e) {
            throw new RuntimeException("store app config failed");
        }
    }
}
