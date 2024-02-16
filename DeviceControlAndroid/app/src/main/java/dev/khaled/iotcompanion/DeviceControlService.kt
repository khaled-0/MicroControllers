package dev.khaled.iotcompanion

import android.app.PendingIntent
import android.content.Intent
import android.os.Build
import android.service.controls.Control
import android.service.controls.ControlsProviderService
import android.service.controls.DeviceTypes
import android.service.controls.actions.ControlAction
import android.service.controls.templates.StatelessTemplate
import android.util.Log
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import java.net.URL
import java.util.concurrent.Flow
import java.util.function.Consumer


class DeviceControlService : ControlsProviderService() {

    private val controlList: List<Control>
        get() = listOf(
            Control.StatelessBuilder(
                "light", PendingIntent.getActivity(
                    this,
                    4,
                    Intent(),
                    PendingIntent.FLAG_CANCEL_CURRENT or PendingIntent.FLAG_IMMUTABLE
                )
            ).setTitle("Light").setControlId("light").setDeviceType(DeviceTypes.TYPE_LIGHT).build(),

            Control.StatelessBuilder(
                "fan", PendingIntent.getActivity(
                    this,
                    5,
                    Intent(),
                    PendingIntent.FLAG_CANCEL_CURRENT or PendingIntent.FLAG_IMMUTABLE
                )
            ).setTitle("Fan").setControlId("fan").setDeviceType(DeviceTypes.TYPE_FAN).build()
        )


    override fun createPublisherFor(controlIds: MutableList<String>): Flow.Publisher<Control> {
        return Flow.Publisher {
            for (control in controlList) {
                if (!controlIds.contains(control.controlId)) continue

                it.onSubscribe(object : Flow.Subscription {
                    override fun cancel() {
                        Log.d("ControlTest", "cancel")
                    }

                    override fun request(p0: Long) {
                        Log.d("ControlTest", "request $p0")
                    }

                })

                it.onNext(
                    Control.StatefulBuilder(control).setStatus(Control.STATUS_OK)
                        .setControlTemplate(StatelessTemplate(control.controlId)).apply {
                            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                                this.setAuthRequired(false)
                            }
                        }.build()
                )

            }
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    override fun performControlAction(
        controlId: String, action: ControlAction, consumer: Consumer<Int>
    ) {
        Log.d(
            "ControlTest", "performControlAction $controlId $action $consumer"
        )

        if (controlId == "light") {
            GlobalScope.launch(Dispatchers.IO) {
                URL("http://esp8266.lan/4/toggle").readText()

            }
        }

        if (controlId == "fan") {
            GlobalScope.launch(Dispatchers.IO) {
                URL("http://esp8266.lan/5/toggle").readText()
            }
        }

        consumer.accept(ControlAction.RESPONSE_OK)

    }

    override fun createPublisherForAllAvailable(): Flow.Publisher<Control> {
        return Flow.Publisher {
            for (control in controlList) it.onNext(control)
            it.onComplete()
        }
    }
}